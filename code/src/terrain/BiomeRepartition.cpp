/**
 * @file BiomeRepartition.cpp
 *
 * @see BiomeRepartition.hpp
 */

#include <cmath>

#include "../../include/Utils.hpp"
#include "../../include/math/InterpolationFunctions.hpp"
#include "../../include/terrain/BiomeRepartition.hpp"

/**
 * @brief
 * Defines the mathematical function "maximum of two numbers".
 *
 * @param a First number.
 * @param b Second number.
 */
#define MAX(a,b) (((a)<(b))?(b):(a))


/**
 * @brief
 * This function defines a pseudo-probability law.
 * It's used to pick if a biome is a land biome or a water biome,
 * with a higher probability of being a land at the center of the map.
 *
 * @param distance The distance to the center of the map of the Voronoi cell.
 * @param size The size of the map.
 * @return The pseudo-probability of the biome being a Land biome.
 */
float landRepartitionProbability(float distance, float size){
    return 1.5f*smooth6Interpolation(distance, size);
}

void computeLand(
    MapParameters& parameters,
    std::vector<Seed>& seeds,
    float mapSize
) 
{
    // Probability of being a land
    float pLand;
    
    // Edges of the map
    float xMin = 0.0f;
    float xMax = mapSize;
    float yMin = 0.0f;
    float yMax = mapSize;

    // Computing iteratively the affectation land/see
    for (
        auto currentSeedIt = seeds.begin(); 
        currentSeedIt != seeds.end();
        currentSeedIt++
    ) {

        // Computing the neighbours
        std::vector<int> neighbours;
        auto currentCell = currentSeedIt->getCell();
        currentCell->neighbors(neighbours);


        double landVolume       = 0.0;
        double neighboursVolume = 0.0;
        bool   neighbourFound   = false;

        // Computing the influence of the neighbours, weighted by their volume
        for (
            auto currentIndexIt = neighbours.begin();
            currentIndexIt != neighbours.end();
            currentIndexIt++
        ) {

	    
	    // If we are on the edge of the map,  then it must be a sea

	    // Getting the needed informations
	    float seedX = currentSeedIt->getX();
	    float seedY = currentSeedIt->getY();
	    // Testing with each edges
	    bool touchTop    = currentCell->plane_intersects( 0.0,  1.0, 0.0, 
							      yMax  - seedY);
	    bool touchBottom = currentCell->plane_intersects( 0.0, -1.0, 0.0, 
							      seedY - yMin);
	    bool touchRight  = currentCell->plane_intersects( 1.0,  0.0, 0.0, 
							      xMax  - seedX);
	    bool touchLeft   = currentCell->plane_intersects(-1.0,  0.0, 0.0, 
							     seedX - xMin);
	    // If the seed touches one of the edges, then it is a sea biome
	    if (touchTop || touchBottom || touchRight || touchLeft) {
		currentSeedIt->setBiome(Sea);
	    } else {
		// else computing the probability of being a land

		// If the index of the neighbour is negative, then it is a
		// wall and not a cell (according to Voro++ specifications).
		if (*currentIndexIt >= 0) {
		    Seed currentNeighbour = seeds[*currentIndexIt];
		    Biome neighbourBiome = currentNeighbour.getBiome();

		    // If the neighbour has been computed, 
		    // Then it should influence the current biome
		    if (neighbourBiome != Undefined) {
			neighbourFound = true;
			double volume = currentNeighbour.getCell()->volume();
			neighboursVolume += volume;
			// Positive influence if it's a land 
			// (here representated by Plains)
			if (neighbourBiome == Plains)
			    landVolume += volume;
		    }
		}
	    

		// Computing the probability dependinf on the distance
		float dx = currentSeedIt->getX() - mapSize/2.0f;
		float dy = currentSeedIt->getY() - mapSize/2.0f;
		float distance = sqrt(dx*dx + dy*dy);
		// Nb : In the next call, it's mapSize and not mapSize/2.0f
		// To make sure that the land will be big enough
		float prob = landRepartitionProbability(distance, mapSize);

		// Computing the blended 'probability'
		if (neighbourFound) {
		    pLand = parameters.getLandBlendingCoefficient()*prob
			+ (1 - parameters.getLandBlendingCoefficient())*(landVolume/neighboursVolume);
		} else {
		    pLand = prob;
		}
        
		// Picking if it's a land
		if ((random(0,1)) <= pLand) {
		    currentSeedIt->setBiome(Plains);
		} else {
		    currentSeedIt->setBiome(Sea);
		}
	    }
	}
    }
}

void computeLake(
    MapParameters& parameters, 
    std::vector<Seed>& seeds,
    std::vector<glm::vec2>& lakes
) 
{
    // Plains that are surrounded by land can turn into a lake

    // Skipping some biomes at the beginning    
    bool  startPick = false;
    float probStart = 1.0;

    for (auto currentSeedIt = seeds.begin();
            currentSeedIt != seeds.end();
            currentSeedIt++) {
   
        // Skipping bloc 
        if (!startPick) {
            probStart *= parameters.getLakeGeometricPicking();
            startPick = ((random(0.0, 1.0)) > probStart);
            if (!startPick)
                continue;
        }

        // Starting to put lakes
        Biome currentBiome = currentSeedIt->getBiome();
	
	// If it's not a land, it cannot turn into a lake
	if (currentBiome != Plains) continue;

        // Otherwise, if the seed is deep in the land, 
        // it might become a lake

        // Computing the neighbours
        std::vector<int> neighbours;
        currentSeedIt->getCell()->neighbors(neighbours);
        
        // Searching if it is a correct neighbourhood
        bool validNeighbourhood = true;
	float probOffset = 0.0f;
        for (auto currentNeighbourIt = neighbours.begin();
                (currentNeighbourIt != neighbours.end()) && validNeighbourhood;
                currentNeighbourIt++ ) {
            int neighbourIndex = *currentNeighbourIt;
            if (neighbourIndex >= 0) {
                Biome neighbourBiome = seeds[neighbourIndex].getBiome();
                validNeighbourhood = ((neighbourBiome == Plains)
				      || (neighbourBiome == Lake));
		// Positive influence
		if (neighbourBiome == Lake)
		    probOffset += parameters.getLakePositiveInfluence();
            }
        }

        // If the neighbourhood is valid, then picking if it becomes a lake
		if (
				(validNeighbourhood)
			&&  ((random(0.0, 1.0)) <= (parameters.getLakeProbTransform() + probOffset))
		) {
			currentSeedIt->setBiome(Lake);
			lakes.push_back(glm::vec2(
								currentSeedIt->getCentroidX(),
								currentSeedIt->getCentroidY()
							)
			);
		}
    }

}




// Private function
// Enable a beach to turn its land neighbours into beaches

void propagateBeach(std::vector<Seed>& seeds, int beachIndex) {
    // Get the cell corresponding to the peak
    auto currentCell = seeds[beachIndex].getCell();

    // Get its neighbours
    std::vector<int> neighbours;
    currentCell->neighbors(neighbours);

    // Turn them into beaches
    for (auto neighboursIdPtr = neighbours.begin();
	 neighboursIdPtr != neighbours.end();
	 neighboursIdPtr++) {
	
	int neighbourId = *neighboursIdPtr;
	if ((neighbourId >= 0) && (seeds[neighbourId].getBiome() == Plains))
	    seeds[neighbourId].setBiome(InnerBeach);
    }
}

void computeBeach(std::vector<Seed>& seeds, float mapSize){

    // The sea type must be propagated towards the interior of the map
    // Land type becomes beach if one of their neighbor of degree 1 or 2 is a sea
    // Small inner seas are erased
    for (int currentSeedIndex = seeds.size()-1; 
	 currentSeedIndex >= 0;
	 currentSeedIndex--) {
	
	Seed currentSeed = seeds[currentSeedIndex];

	Biome currentBiome = currentSeed.getBiome();

	bool seaFound = false;

	// Computing the neighbors
	auto currentCell = currentSeed.getCell();
	std::vector<int> neighbours;
	currentCell->neighbors(neighbours);
	
	// Searching a sea in the neighbors
	for (auto currentNeighbourIndexIt = neighbours.begin();
	     (currentNeighbourIndexIt != neighbours.end()) && (!seaFound);
	     currentNeighbourIndexIt++) { 
	    int currentNeighbourIndex = *currentNeighbourIndexIt;
	    if (currentNeighbourIndex >= 0)
		seaFound = ((seeds[currentNeighbourIndex].getBiome()) == Sea);
	}
	// Setting the new biome
	if (seaFound) {
	    switch (currentBiome) {
		
	    case Plains :
	    case InnerBeach:
		// Default land type turn into a beach
		seeds[currentSeedIndex].setBiome(OutterBeach);
		// Propagate the beach 
		propagateBeach(seeds, currentSeedIndex);
		break;
		
	    default:
		break;
	    }
	} else {
	    switch (currentBiome) {
		
	    case Sea :
		// Inner sea are erased
		currentSeed.setBiome(Plains);
		break;

	    default :
		break;
	    }
	}
    }
}


// Private function
// Used here so a peak can raise its neighbours into mountains
void raiseMountains(std::vector<Seed>& seeds, int peakIndex) {

    // Get the cell corresponding to the peak
    auto currentCell = seeds[peakIndex].getCell();

    // Get its neighbours
    std::vector<int> neighbours;
    currentCell->neighbors(neighbours);

    // Turn them into mountains
    for (auto neighboursIdPtr = neighbours.begin();
	 neighboursIdPtr != neighbours.end();
	 neighboursIdPtr++) {
	
	int neighbourId = *neighboursIdPtr;
	if ((neighbourId >= 0) && (seeds[neighbourId].getBiome() != Peak))
	    seeds[neighbourId].setBiome(Mountain);
    }
}


void computeMountains(MapParameters& parameters, std::vector<Seed>& seeds){

    int nbBiomes = seeds.size();

    // First step : Picking the initial peak
    // Nb : The first biome might inconditionnaly become a peak
    int peakIndex = 0;
    float probPick = 1.0;
    while ((random(0.0, 1.0) <= probPick) && (peakIndex < nbBiomes/2)) {
	// Checking if the picked biome can be a peak
	if (seeds[peakIndex+1].getBiome() != Plains)
	    break;
	peakIndex++;
	probPick *= parameters.getMountainGeometricPicking();
    }
    seeds[peakIndex].setBiome(Peak);
    // Turning its neighbours into mountains (hills actually)
    raiseMountains(seeds, peakIndex);
    

    // Second step : propagating the mountain
    int nbPeak = 1;
    probPick = 1.0;
    // First picking if a new peak is created
    while ((random(0.0, 1.0) <= probPick) && (nbPeak < nbBiomes/2)) {
	nbPeak++;
	probPick *= parameters.getMountainProbTransform();
	// Computing the neighbours
	auto currentCell = seeds[peakIndex].getCell();
	std::vector<int> neighbours;
	currentCell->neighbors(neighbours);
	int nbNeighbours = neighbours.size();
	// Picking the neighbour that is going to be a Peak
	int nbTry = 0;
	bool invalidNeighbour;
	do {
	    nbTry++;
	    peakIndex = neighbours[rand() % nbNeighbours];
	    // The neighbour, if the index is valid, is compulsorily a mountain
	    invalidNeighbour = (peakIndex < 0);
	} while (invalidNeighbour && (nbTry < parameters.getMountainMaxTry()));
	// breaking if we cannot find a suitable neighbour
	if (invalidNeighbour)
	    break;
	// Setting the peak
	seeds[peakIndex].setBiome(Peak);
	// Transforming its neighbours
	raiseMountains(seeds, peakIndex);
    }
}
