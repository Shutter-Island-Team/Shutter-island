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

float landRepartitionProbability(float distance, float size){
    return 1.1f*blobSmooth6(distance, size, 1.0);
}

void computeCoast(std::vector<Seed> seeds, float mapSize) {
				      
    // Computing iteratively the affectation land/see
    for (auto currentSeedIt = seeds.begin(); 
	 currentSeedIt != seeds.end();
	 currentSeedIt++) {
	
	// Computing the neighbours
	std::vector<int> neighbours;
	currentSeedIt->neighbors(neighbours);

	double landVolume       = 0.0;
	double neighboursVolume = 0.0;
	bool   neighbourFound   = false;
	
	// Computing the influence of the neighbours
	for (auto currentIndexIt = neighbours.begin();
	     currentIndexIt != neighbours.end();
	     currentIndexIt++) {

	    Seed currentNeighbour = seeds[*currentIndexIt];
	    Biome neighbourBiome = currentNeighbour.getBiome();

	    // If the neighbour has been computed, 
	    // Then it should influence the current biome
	    if (neighbourBiome != Undefined) {
		neighbourFound = true;
		double volume = currentNeighbour.volume();
		neighboursVolume += volume;
		// Positive influence if it's a land 
		// (here representated by Plains)
		if (neighbourBiome == Plains)
		    landVolume += volume;
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
