/**
 * @file MapGenerator.cpp
 *
 * @see MapGenerator.hpp
 */

/**
 * @brief
 * Min function between two numbers.
 *
 * @param a First number.
 * @param b Second number.
 */
#define MIN(a, b) (((a)<(b))?(a):(b))

/**
 * @brief
 * Max function between two numbers.
 *
 * @param a First number.
 * @param b Second number.
 */
#define MAX(a, b) (((a)>(b))?(a):(b))


#include "../../include/terrain/Seed.hpp"
#include "../../include/terrain/MapGenerator.hpp"
#include "../../include/terrain/MapUtils.hpp"
#include "../../include/terrain/BiomeRepartition.hpp"
#include <iostream>

MapGenerator::MapGenerator(MapParameters& parameters, float size) :
    m_mapParameters(parameters),
    mapSize{size},
    voronoiSeedsGenerator{VoronoiSeedsGenerator(size, size,
						parameters.getNbSeeds(),
						parameters.getNbSubdivision(),
						parameters.getNbSubdivision(),
						parameters.getNbSeedsMaxSubdiv(),
						parameters.getDistMin())},
    /**
     * @brief
     * Voronoi diagram's cells container Constructor, define in the voro++
     * library.
     *
     * @see voro++ library.
     */
    seedsContainer{0.0, size+1, 0.0, size+1, -10.0, 10.0,
	    parameters.getNbSubdivision(), parameters.getNbSubdivision(), 1,
	    false, false, false,
	    1}
{}

MapGenerator::~MapGenerator() { 
    if (heightTree) {
        heightTree->freeHeightTree();
        heightTree = NULL;
    }
    if (biomeMap) {
	delete [] biomeMap;
    }
}
 

void MapGenerator::compute() {

    // Position Generation
    /*
     * Note that the seeds are ordered by their distance to the center of 
     * the map.
     * This will be useful for the biome repartition step.
     */
    voronoiSeedsGenerator.generateSeeds(seeds);

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

    // Repartition land/sea
    computeLand(m_mapParameters, seeds, mapSize);

    // Computing the beaches depending on the seas
    computeBeach(seeds, mapSize);

    // Mountain repartition
    computeMountains(m_mapParameters, seeds);

    // Adding the lakes
    computeLake(m_mapParameters, seeds, m_lakes);


    // HeightTree step
    // Creating the initial map : a deep dark sea
    HeightData tlCorner(Vertex2D(0.0f,    mapSize), biomeHeight(m_mapParameters, Sea), Sea);
    HeightData trCorner(Vertex2D(mapSize, mapSize), biomeHeight(m_mapParameters, Sea), Sea);
    HeightData blCorner(Vertex2D(0.0f,    0.0f)   , biomeHeight(m_mapParameters, Sea), Sea);
    HeightData brCorner(Vertex2D(mapSize, 0.0f)   , biomeHeight(m_mapParameters, Sea), Sea);
    heightTree = new HeightTree(m_mapParameters, 
				HeightNode(m_mapParameters, mapSize,
					   tlCorner, trCorner,
					   blCorner, brCorner));
    // Computing the tree
    heightTree->computeTree(seedsContainer, seeds);
    

    // Biome map 
    // This sampled biome map is used to accelerate the search of a biome associated
    // to a position, altough the result is obviously approximative

    // Filling the biome map
    int heightmapScaling    = this->m_mapParameters.getHeightmapScaling();
    int mapSize             = (int) this->mapSize;
    int effMapSize          = mapSize*heightmapScaling;
    biomeMap = new Biome [effMapSize*effMapSize];
    // pragma omp does not work here...
    for (int i = 0; i < effMapSize; i++) {
	for (int j = 0; j < effMapSize; j++) {
	    float effI = (float)i / (float)heightmapScaling;
	    float effJ = (float)j / (float)heightmapScaling;
	    biomeMap[i+j*effMapSize] = this->getBiome(effI, effJ);
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
    
    Vertex2D position = clipPosition(x, y);

    int heightmapScaling    = this->m_mapParameters.getHeightmapScaling();
    int mapSize             = (int) this->mapSize;
    int effMapSize          = mapSize*heightmapScaling;

    return findApproximativeBiome(position, effMapSize, biomeMap, heightmapScaling);
}

void MapGenerator::getCentroid(float x, float y, 
			       float& xCentroid, float& yCentroid) {
    Vertex2D position = clipPosition(x, y);
    return findClosestCentroid(position, seedsContainer, seeds,
			       xCentroid, yCentroid);
}

float MapGenerator::getHeight(float x, float y) {

    if (!heightTree) { 
	std::cerr << "HeightTree not computed !" << std::endl;
	exit(EXIT_FAILURE);
    }

    Vertex2D position = clipPosition(x, y);
    
    int heightmapScaling    = this->m_mapParameters.getHeightmapScaling();
    int mapSize             = (int) this->mapSize;
    int effMapSize          = mapSize*heightmapScaling;
    return heightTree->evalHeight(position, 
				  effMapSize,
				  biomeMap);
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
    } else {
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
