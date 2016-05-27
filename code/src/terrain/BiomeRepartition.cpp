/**
 * @file BiomeRepartition.cpp
 *
 * @see BiomeRepartition.hpp
 */

#include <cmath>

#include "../../include/Utils.hpp"
#include "../../include/math/InterpolationFunctions.hpp"
#include "../../include/terrain/BiomeRepartition.hpp"

#define MAX(a,b) (((a)<(b))?(b):(a))

#define LAND_BLENDING_COEFFICIENT (0.7f)

#define LAKE_GEOMETRIC_PICKING  (0.8)
#define LAKE_PROB_TRANSFORM     (0.1)
#define LAKE_POSITIVE_INFLUENCE (0.15f)

#define MOUNTAIN_GEOMETRIC_PICKING (0.7)
#define MOUNTAIN_PROB_TRANSFORM    (0.9)
#define MOUNTAIN_MAX_TRY            (10)


// This function defines a pseudo-probability law
// It's use to pick if a biome is a land biome or a water biome
// With a higher probability of being a land at the center of the map
float landRepartitionProbability(float distance, float size){
    return 1.1f*smooth6Interpolation(distance, size);
}

void computeLand(std::vector<Seed>& seeds, float mapSize) {

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
		    pLand = LAND_BLENDING_COEFFICIENT*prob
			+ (1 - LAND_BLENDING_COEFFICIENT)*(landVolume/neighboursVolume);
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

void computeLake(std::vector<Seed>& seeds) {

    // Plains that are surrounded by land can turn into a lake

    // Skipping some biomes at the beginning    
    bool  startPick = false;
    float probStart = 1.0;

    for (auto currentSeedIt = seeds.begin();
            currentSeedIt != seeds.end();
            currentSeedIt++) {
   
        // Skipping bloc 
        if (!startPick) {
            probStart *= LAKE_GEOMETRIC_PICKING;
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
		    probOffset += LAKE_POSITIVE_INFLUENCE;
            }
        }

        // If the neighbourhood is valid, then picking if it becomes a lake
        if ((validNeighbourhood) 
	    && ((random(0.0, 1.0)) <= (LAKE_PROB_TRANSFORM + probOffset)))
            currentSeedIt->setBiome(Lake);   
    }

}



void computeBeach(std::vector<Seed>& seeds, float mapSize){

    // The sea type must be propagated towards the interior of the map
    // Land type becomes beach if one of their neighbor is a sea
    // Small inner seas are erased
    for (auto currentSeedIt = seeds.rbegin(); 
	 currentSeedIt != seeds.rend();
	 currentSeedIt++) {
	
	Biome currentBiome = currentSeedIt->getBiome();

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
	if (seaFound) {
	    switch (currentBiome) {
		
	    case Plains :
		// Default land type turn into a beach
		currentSeedIt->setBiome(Beach);
		break;
		
	    default:
		break;
	    }
	} else {
	    switch (currentBiome) {
		
	    case Sea :
		// Inner sea are erased
		currentSeedIt->setBiome(Plains);
		break;

	    default :
		break;
	    }
	}
    }
}



void computeMountains(std::vector<Seed>& seeds){

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
    probPick = 1.0;
    // First picking if a new mountain is created
    while ((random(0.0, 1.0) <= probPick) && (nbMountain < nbBiomes-1)) {
	nbMountain++;
	probPick *= MOUNTAIN_PROB_TRANSFORM;
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
