/**
 * @file BiomeRepartition.cpp
 *
 * @see BiomeRepartition.hpp
 */

#include <cmath>

#include "../../include/Utils.hpp"
#include "../../include/terrain/BlobFunctions.hpp"
#include "../../include/terrain/BiomeRepartition.hpp"

#define LAND_BLENDING_COEFFICIENT (0.7f)

#define MOUNTAIN_GEOMETRIC_PICKING (0.9)
#define MOUNTAIN_MAX_TRY            (10)

float landRepartitionProbability(float distance, float size){
    return 1.1f*blobSmooth6(distance, size, 1.0);
}

void computeLand(std::vector<Seed>& seeds, float mapSize) {
    // Computing iteratively the affectation land/see
    for (
        auto currentSeedIt = seeds.begin(); 
        currentSeedIt != seeds.end();
        currentSeedIt++
    ) {

        // Computing the neighbours
        std::vector<int> neighbours;

        currentSeedIt->getCell()->neighbors(neighbours);


        double landVolume       = 0.0;
        double neighboursVolume = 0.0;
        bool   neighbourFound   = false;

        // Computing the influence of the neighbours
        for (
            auto currentIndexIt = neighbours.begin();
            currentIndexIt != neighbours.end();
            currentIndexIt++
        ) {

            /*
             * If the index of the neighbour is negative, then it is a
             * wall and not a cell (according to Voro++ specifications).
             */
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
        }

        // Computing the probability of being a land

        float dx = currentSeedIt->getX() - mapSize/2.0f;
        float dy = currentSeedIt->getY() - mapSize/2.0f;
        float distance = sqrt(dx*dx + dy*dy);
        float pLand = LAND_BLENDING_COEFFICIENT*landRepartitionProbability(distance, mapSize)
            + (1 - LAND_BLENDING_COEFFICIENT)*(landVolume/neighboursVolume);

        // Picking if it's a land
        if ((random(0,1)) <= pLand) {
            currentSeedIt->setBiome(Plains);
        } else {
            currentSeedIt->setBiome(Sea);
        }
    }
}

void splitWater(std::vector<Seed>& seeds, float mapSize){

    float xMin = 0.0f;
    float xMax = mapSize;
    float yMin = 0.0f;
    float yMax = mapSize;

    // First step : the seeds that touch the edges of the map must be set to sea type
    for (auto currentSeedIt = seeds.begin(); 
	 currentSeedIt != seeds.end();
	 currentSeedIt++) {
	// Getting the needed informations
	float seedX = currentSeedIt->getX();
	float seedY = currentSeedIt->getY();
	auto currentCell = currentSeedIt->getCell();
	// Testing with each edges
	bool touchTop    = currentCell->plane_intersects( 0.0,  1.0, 0.0, yMax  - seedY);
	bool touchBottom = currentCell->plane_intersects( 0.0, -1.0, 0.0, seedY - yMin);
	bool touchRight  = currentCell->plane_intersects( 1.0,  0.0, 0.0, xMax  - seedX);
	bool touchLeft   = currentCell->plane_intersects(-1.0,  0.0, 0.0, seedX - xMin);
	// If the seed touches one of the edges, then it is a sea biome
	if (touchTop || touchBottom || touchRight || touchLeft)
	    currentSeedIt->setBiome(Sea);
    }

    // Second step : the sea type must be propagated towards the interior of the map
    // Water type becomes sea if one of their neighbor is a sea
    // Land type  becomes beach if one of their neighbor is a sea
    for (auto currentSeedIt = seeds.rbegin(); 
	 currentSeedIt != seeds.rend();
	 currentSeedIt++) {
	
	Biome currentBiome = currentSeedIt->getBiome();

	// Nothing to do if it's a sea
	if (currentBiome == Sea) 
	    continue;

	bool seaFound = false;

	// Computing the neighbors
	auto currentCell = currentSeedIt->getCell();
	std::vector<int> neighbours;
	currentCell->neighbors(neighbours);
	
	// Searching a sea in the neighbors
	for (auto currentIndexIt = neighbours.begin();
	     (currentIndexIt != neighbours.end()) && (!seaFound);
	     currentIndexIt++) { 
	    int currentIndex = *currentIndexIt;
	    if (currentIndex >= 0)
		seaFound = ((seeds[currentIndex].getBiome()) == Sea);
	}
	// Setting the new biome
	if (seaFound)
	    switch (currentBiome) {
		
	    case Lake :
		// Default water type
		currentSeedIt->setBiome(Sea);
		break;

	    case Plains :
		// Default land type
		currentSeedIt->setBiome(Beach);
		break;
		
	    default:
		break;
	    }
    }
}



void raiseMountains(std::vector<Seed>& seeds, float p){

    int nbBiomes = seeds.size();

    // First step : Picking the initial mountain
    // Nb : The first biome might inconditionnaly become a mountain
    int mountainIndex = 0;
    float probPick = 1.0;
    while ((random(0.0, 1.0) <= probPick) && (mountainIndex < nbBiomes-1)) {
	// Checking if the picked biome can be mountain
	if (seeds[mountainIndex+1].getBiome() != Plains)
	    break;
	mountainIndex++;
	probPick *= MOUNTAIN_GEOMETRIC_PICKING;
    }
    seeds[mountainIndex].setBiome(Mountain);
    
    // Second step : propagating the mountain
    int nbMountain = 1;
    probPick = p;
    // First picking if a new mountain is created
    while ((random(0.0, 1.0) <= probPick) && (nbMountain < nbBiomes-1)) {
	nbMountain++;
	probPick *= p;
	// Computing the neighbours
	auto currentCell = seeds[mountainIndex].getCell();
	std::vector<int> neighbours;
	currentCell->neighbors(neighbours);
	int nbNeighbours = neighbours.size();
	// Picking the neighbour that is going to be a Mountain
	int nbTry = 0;
	bool invalidNeighbour;
	do {
	    nbTry++;
	    mountainIndex = neighbours[rand() % nbNeighbours];
	    invalidNeighbour = (mountainIndex < 0) || (seeds[mountainIndex].getBiome() != Plains);
	} while (invalidNeighbour && (nbTry < MOUNTAIN_MAX_TRY));
	// breaking if we cannot find a suitable neighbour
	if (invalidNeighbour)
	    break;
	seeds[mountainIndex].setBiome(Mountain);
    }
	

}
