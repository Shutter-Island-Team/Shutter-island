/**
 * @file MapGenerator.cpp
 *
 * @see MapGenerator.hpp
 */

/*
							/!\ CAUTION /!\
	An important point has to be underline once and for all.
	
	There is a "shift" between matrix coordinates (I, J) and plane coordinates
	(x, y), which may lead to a misunderstanding and/or confusion when the
	plane is divided thanks to a grid.
	
	As a matter of fact, a grid's - assimilated to a matrix - rows correspond
	to a segmentation of the ordinate axis of the plane (respectively, its
	columns correspond to a segmentation of the abscissa axis of the plane).
	
	That is why, in some parts of the below code, when sampling the heightmap 
	for example, we have to call "getHeight(effJ, effI)" since the sampling
	is done using an underlaying (virtual) grid, and "getHeight" needs plane
	coordinates.
*/

/*
	In order to create a directory, we have to use system-specific modules
	(we chose not to use Boost::Filesystem because of its heaviness and 
	its needing to be built beforehand.)
*/
#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include "../../include/terrain/BiomeRepartition.hpp"
#include "../../include/terrain/MapGenerator.hpp"
#include "../../include/terrain/MapParser.hpp"
#include "../../include/terrain/MapUtils.hpp"
#include "../../include/terrain/Seed.hpp"

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

MapGenerator::MapGenerator(MapParameters& parameters, float size) :
    m_mapParameters(parameters),
    mapSize{ size },
    voronoiSeedsGenerator{ VoronoiSeedsGenerator(size, size,
                        parameters.getNbSeeds(),
                        parameters.getNbSubdivision(),
                        parameters.getNbSubdivision(),
                        parameters.getNbSeedsMaxSubdiv(),
                        parameters.getDistMin()) },
    /**
     * @brief
     * Voronoi diagram's cells container Constructor, define in the voro++
     * library.
     *
     * @see voro++ library.
     */
    seedsContainer{ 0.0, size + 1, 0.0, size + 1, -10.0, 10.0,
        parameters.getNbSubdivision(), parameters.getNbSubdivision(), 1,
        false, false, false,
        1 }
{}

MapGenerator::~MapGenerator()
{
	if (heightTree) {
		heightTree->freeHeightTree();
		heightTree = NULL;
	}
	if (biomeMap) {
		delete[] biomeMap;
		heightMap = NULL;
	}
	if (heightMap) {
		delete[] heightMap;
		heightMap = NULL;
	}
}


void MapGenerator::compute() {

    // Position Generation
    /*
     * Note that the seeds are ordered by their distance to the center of
     * the map.
     * This will be useful for the biome repartition step.
     */
	if (m_mapParameters.getImportingSeeds()) {
		/*
			Import may take some time.
			Thus, we notify the user.
		*/
		std::cout << "\nStarting import of the seeds." << std::endl;
		std::cout << "It may take up to 5 minutes for vast maps.\n" << std::endl;

		/*
			If the data concerning the seeds are imported, two main steps are
			done:
				- Getting the number of seeds from the map parameters so as to 
				resize the vector which is going to contain them;
				- Parsing the seeds' data and creating them.
			For more information concerning the file format used to export
			and import map data, refer to the global project README.md.
		*/
		std::string seedsFilename = "../mapData/" +
										m_mapParameters.getImportSeeds();
		MapParser parser(
			seedsFilename.c_str(), 
			m_mapParameters.getParserBufferSize()
		);

		/*
			First step => getting the number of seeds and resizing the vector.
		*/
		int nbOfSeeds = m_mapParameters.getNbSeeds();
		seeds.resize(nbOfSeeds);

		/*
			Second step => parsing the seeds and filling the vector of seeds
			with the parsed data.
		*/
		int currentSeed = 0;
		while ((currentSeed < nbOfSeeds) && !parser.hasReachedEOF()) {
			parser.skipWhiteCharacters();
			if (!parser.isComment('#', true)) {
				parser.parseSeed(seeds[currentSeed]);
				currentSeed++;
			}
		}

		if (currentSeed < nbOfSeeds) {
			std::cerr << "Importing seeds' data => Not enough seeds to parse";
			std::cerr << std::endl;
			exit(EXIT_FAILURE);
		} else {
			/*
				Notifying the user that the import of the seeds has been
				successfully done.
			*/
			std::cout << "The seeds have been successfully imported.\n";
			std::cout << std::endl;
		}
	} else {
		voronoiSeedsGenerator.generateSeeds(seeds);
	}
    // Voronoi step
    /*
     * Adding all the seeds to the container so as to generate Voronoi
     * diagram.
     * Since voro++ aims at computing tridimensional Voronoi diagrams, and
     * we only are interested in bidimensional ones, we decided to set the
     * third dimension to "0.0", so that we may simulate a bidimensional
     * behavior of voro++.
     * In order to properly loop over the seeds during the repartition step,
     * we need to sort them according to the distance of the latter to a
     * reference point (typically the middle of the map).
     * That is why we use a "particle_order" and a "loop_order" to retain
     * this order.
     */
    voro::particle_order seedsOrder;
    int ID = 0;
    for (
        auto iterator = seeds.begin();
        iterator != seeds.end();
        iterator++, ID++
        ) {
        seedsContainer.put(
            seedsOrder,
            ID,
            iterator->getX(),
            iterator->getY(),
            0.0
        );
    }

    voro::c_loop_order loopOrder(seedsContainer, seedsOrder);
    loopOrder.start();
    for (
        auto iterator = seeds.begin();
        iterator != seeds.end();
        iterator++, loopOrder.inc()
        ) {
        voroNeighborPtr currentCellPtr = std::make_shared<voro::voronoicell_neighbor>();
        seedsContainer.compute_cell(
            *currentCellPtr,
            loopOrder
        );
        iterator->setCell(currentCellPtr);
        /*
         * Setting the centroids coordinates into the seed.
         */
        double x, y, z;
        currentCellPtr->centroid(x, y, z);
        iterator->setCentroidX(x + iterator->getX());
        iterator->setCentroidY(y + iterator->getY());
    }

    // Biome step
	/*
		If the seeds' data are importerd, this step has to be shunted.
	*/
	if (!m_mapParameters.getImportingSeeds()) {
		// Repartition land/sea
		computeLand(m_mapParameters, seeds, mapSize);

		// Computing the beaches depending on the seas
		computeBeach(seeds, mapSize);

		// Mountain repartition
		computeMountains(m_mapParameters, seeds);

		// Adding the lakes
		computeLake(m_mapParameters, seeds, m_lakes);
	}


	if (!m_mapParameters.getImportingHeightmap()) {
		// HeightTree step
		// Creating the initial map : a deep dark sea
		HeightData tlCorner(Vertex2D(0.0f, mapSize), biomeHeight(m_mapParameters, Sea), Sea);
		HeightData trCorner(Vertex2D(mapSize, mapSize), biomeHeight(m_mapParameters, Sea), Sea);
		HeightData blCorner(Vertex2D(0.0f, 0.0f), biomeHeight(m_mapParameters, Sea), Sea);
		HeightData brCorner(Vertex2D(mapSize, 0.0f), biomeHeight(m_mapParameters, Sea), Sea);
		heightTree = new HeightTree(m_mapParameters,
			HeightNode(m_mapParameters, mapSize,
				tlCorner, trCorner,
				blCorner, brCorner));
		// Computing the tree
		heightTree->computeTree(seedsContainer, seeds);
	}
    
    // Biome map and height map
    // These sampled maps are used to accelerate the search of a biome or of a height associated
    // to a position, altough the result is obviously approximative
    // Filling the biome map and the height map
    int heightmapScaling    = this->m_mapParameters.getHeightmapScaling();
    int mapSize             = (int) this->mapSize;
    int effMapSize          = mapSize*heightmapScaling;
	int nbOfPoints			= effMapSize*effMapSize;
    biomeMap  = new Biome [nbOfPoints];
    heightMap = new float [nbOfPoints];
    
	/*
		If importing the map data, then, the sampled approximative map is read
		from the import file.
	*/
	if (m_mapParameters.getImportingHeightmap()) {
		/*
			Import may take some time.
			Thus, we notify the user.
		*/
		std::cout << "\nStarting import of the heightmap." << std::endl;
		std::cout << "It may take up to 5 minutes for vast maps.\n" << std::endl;

		/*
			For each sampled point of the imported heightmap, its plane 
			coordinates are appended as before the associated
			height.
			In consequence, parsing a sampled point contains two steps:
				- Parsing the plane coordinates (in fact, they correspond to
				the effective coordinate after rescaling, "effI" and "effJ");
				- Parsing the height (a float).
			By exporting and importing the effective coordinates too, we can
			compensate a bit the extra computation time needed for file
			handling by suppressing 2 floating-point divisions by iteration.
		*/
		std::string hmapFilename = "../mapData/" +
										m_mapParameters.getImportHeightmap();
		MapParser parser(
			hmapFilename.c_str(),
			m_mapParameters.getParserBufferSize()
		);

		int currentSampledPoint = 0;
		while (( currentSampledPoint< nbOfPoints) && !parser.hasReachedEOF()) {
			parser.skipWhiteCharacters();
			if (!parser.isComment('#', true)) {
				float effI = parser.parseFloat();
				float effJ = parser.parseFloat();
				
				biomeMap[currentSampledPoint] = this->getBiome(effJ, effI);
				heightMap[currentSampledPoint] = parser.parseFloat();
				currentSampledPoint++;
			}
		}

		/*
			Notifying the user that the import of the heightmap has been
			successfully done.
		*/
		std::cout << "The heightmap has been successfully imported.\n"; 
		std::cout << std::endl;

	} else {
		// pragma omp does not work here...
		for (int i = 0; i < effMapSize; i++) {
			for (int j = 0; j < effMapSize; j++) {
				float effI = (float)i / (float)heightmapScaling;
				float effJ = (float)j / (float)heightmapScaling;
				biomeMap[j + i*effMapSize] = this->getBiome(effJ, effI);
			}
		}
		// Filling now the height map
		// TODO Here replace the computation of the height map
		// pragma omp does not work here...
		for (int i = 0; i < effMapSize; i++) {
			for (int j = 0; j < effMapSize; j++) {
				float effI = (float)i / (float)heightmapScaling;
				float effJ = (float)j / (float)heightmapScaling;
				Vertex2D pos(effJ, effI);
				heightMap[j + i*effMapSize] = heightTree->evalHeight(pos, effMapSize, biomeMap);
			}
		}
	}
}

// To ensure that voro++ does not raise an exception, we
// clip the positions given to voro++ to the space
// allowed by the library
Vertex2D MapGenerator::clipPosition(float x, float y) {
    return Vertex2D(MAX(MIN(x, mapSize), 0.0f),
        MAX(MIN(y, mapSize), 0.0f));
}

Biome MapGenerator::getBiome(float x, float y) {
    Vertex2D position = clipPosition(x, y);
    return findClosestBiome(position, seedsContainer, seeds);
}

Biome MapGenerator::getApproximativeBiome(float x, float y) {

    if (!biomeMap) {
        std::cerr << "BiomeMap not computed !" << std::endl;
        exit(EXIT_FAILURE);
    }

    Vertex2D position = clipPosition(x, y);

    int heightmapScaling = this->m_mapParameters.getHeightmapScaling();
    int mapSize = (int) this->mapSize;
    int effMapSize = mapSize*heightmapScaling;

    return findApproximativeBiome(position, effMapSize, biomeMap, heightmapScaling);
}

void MapGenerator::getCentroid(float x, float y,
    float& xCentroid, float& yCentroid) {
    Vertex2D position = clipPosition(x, y);
    return findClosestCentroid(position, seedsContainer, seeds,
        xCentroid, yCentroid);
}

float MapGenerator::getHeight(float x, float y) {

    // Left for debug : Use the height tree
    /*if (!heightTree) {
    std::cerr << "HeightTree not computed !" << std::endl;
    exit(EXIT_FAILURE);
    }

    Vertex2D position = clipPosition(x, y);

    int heightmapScaling    = this->m_mapParameters.getHeightmapScaling();
    int mapSize             = (int) this->mapSize;
    int effMapSize          = mapSize*heightmapScaling;
    return heightTree->evalHeight(position,
                  effMapSize,
                  biomeMap);*/

    if (!heightMap) {
        std::cerr << "HeightMap not computed !" << std::endl;
        exit(EXIT_FAILURE);
    }

    Vertex2D position = clipPosition(x, y);

    int heightmapScaling = this->m_mapParameters.getHeightmapScaling();
    int mapSize = (int) this->mapSize;
    int effMapSize = mapSize*heightmapScaling;

    return findApproximativeHeight(position, effMapSize, heightMap, heightmapScaling);
}


MapParameters& MapGenerator::getMapParameters()
{
    return m_mapParameters;
}

bool MapGenerator::getLakes(
    std::vector<glm::vec2>::iterator& begin,
    std::vector<glm::vec2>::iterator& end
)
{
    if (m_lakes.size() > 0) {
        begin = m_lakes.begin();
        end = m_lakes.end();
        return true;
    }
    else {
        return false;
    }
}

bool MapGenerator::getClosestLake(
    float x,
    float y,
    float& xLake,
    float& yLake
)
{
    return findClosestLake(m_lakes, x, y, xLake, yLake);
}

void MapGenerator::exportMapData()
{
	/*
		If the map has been imported, no need to export it.
	*/
	if	(
			m_mapParameters.getImportingHeightmap() ||
			m_mapParameters.getImportingSeeds()
		) 
	{
		std::cout << "\nThe current map has been imported." << std::endl;
		std::cout << "Thus, there is no need to export it." << std::endl;
		return;
	}

	/*
		Since export may take some time (for instance, a 1000x1000 map with
		300 seeds and no upscaling takes approximatively 20 seconds to export),
		we notify the user.
	*/
	std::cout << "\nStarting export of the map data." << std::endl;
	std::cout << "It may take up to 15 minutes for vast maps.\n" << std::endl;

	/*
		Creating a directory in "mapData" so as to save the exported files
		into the latter.
		The directory name is a timestamp, so as to avoid erasing exported
		data unintentionally.
		Moreover, a portable, lightweight and build-free way to create a
		directory in C++ does not exist ("lightweight and build-free" excludes
		Boost::Filesystem from the potential candidates). This is why
		pre-defined OS macros and system-specific code are used.
	*/
	time_t tstamp = time(NULL);
	std::string directoryName = "../mapData/export_" + std::to_string(tstamp);
	bool directoryCreated = false;
#ifdef __linux__
	int mkdirStatus = mkdir(
		directoryName.c_str(),
		S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
	);
	directoryCreated = (mkdirStatus == 0);
#endif
#ifdef _WIN32
	directoryCreated = CreateDirectory(directoryName.c_str(), NULL);
#endif
	if (!directoryCreated) {
		std::cerr << "exportMapData - Could not create the export directory.";
		std::cerr << std::endl;
		exit(EXIT_FAILURE);
	}

	/*
		Setting up the export of the seeds' data toward a file.
	*/
	std::ofstream outputSeeds;
	std::string seedsFilename = directoryName + "/" + 
									m_mapParameters.getExportSeeds();
	outputSeeds.open(seedsFilename.c_str());
	/*
		Exporting the seeds' data.
	*/
	outputSeeds << "# Seeds' data:" << std::endl;
	for (auto seedsIt = seeds.begin(); seedsIt != seeds.end(); seedsIt++) {
		outputSeeds << seedsIt->getX() << " " << seedsIt->getY() << " ";
		outputSeeds << biomeToString(seedsIt->getBiome()) << std::endl;
	}
	/*
		Closing the exporting stream.
	*/
	outputSeeds.close();

	/*
		Setting up the export of the heightmap's data toward a file.
	*/
	int heightmapScaling = this->m_mapParameters.getHeightmapScaling();
	int mapSize = (int) this->mapSize;
	int effMapSize = mapSize*heightmapScaling;
	
	std::ofstream outputHeightmap;
	std::string heightmapFilename = directoryName + "/" +
									m_mapParameters.getExportHeightmap();
	outputHeightmap.open(heightmapFilename.c_str());
	/*
		Exporting the heightmap's data.
	*/
	outputHeightmap << "#Heightmap data:" << std::endl;
	for (int i = 0; i < effMapSize; i++) {
		for (int j = 0; j < effMapSize; j++) {
			float effI = (float)i / (float)heightmapScaling;
			float effJ = (float)j / (float)heightmapScaling;

			outputHeightmap << effI << " " << effJ << " ";
			outputHeightmap << getHeight(effJ, effI) << std::endl;
		}
	}
	/*
		Closing the export stream.
	*/
	outputHeightmap.close();

	/*
		Copying the JSon parameters file.
	*/
	std::string parametersFilename = directoryName + "/MapParameters.json";
	std::ifstream source("../mapData/MapParameters.json", std::ios::binary);
	std::ofstream copy(parametersFilename.c_str(), std::ios::binary);

	copy << source.rdbuf();

	source.close();
	copy.close();

	/*
		Notifying the user that the export has been successfully done.
	*/
	std::cout << "Export has been successfully done.\n" << std::endl;
}