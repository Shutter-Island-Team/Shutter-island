/**
 * @file Biome.cpp
 *
 * @see Biome.hpp
 */

#include <iostream>

#include "../../include/Utils.hpp"
#include "../../include/terrain/Biome.hpp"



float biomeHeight(Biome biome) {

    // Simply generating a random height depending on the biome
    switch (biome) {
    
    case Undefined :
	std::cerr << "Error : Trying to get the height of an undefined biome" << std::endl;
	exit(1);
	
    case Sea :
    case Lake :
	return random(HEIGHT_MIN_WATER,   HEIGHT_MAX_WATER);

    case Beach :
	return random(HEIGHT_MIN_BEACH,   HEIGHT_MAX_BEACH);

    case Plains :
	return random(HEIGHT_MIN_PLAINS,  HEIGHT_MAX_PLAINS);

    case Mountain :
	return random(HEIGHT_MIN_MOUTAIN, HEIGHT_MAX_MOUTAIN);

    }
}

float computeBlobScale(Biome biome, Biome biomeNeighbour1, Biome biomeNeighbour2) {

    if ((biome = biomeNeighbour1) && (biome = biomeNeighbour2)) {
	// Neighbours are of the same type, we can assume that we are within
	// a large area of this kind
	return BLOB_SCALE_LARGE;
    } else {
	// Else, the influence zone should be smaller
	switch (biome) {
	    
	case Mountain :
	    // Even smaller for moutains
	    return BLOB_SCALE_SMALL;

	default :
	    return BLOB_SCALE_MEDIUM;
	}
    }
}



bool checkSubdivision(int currentDepth) {
    return (currentDepth <= BIOME_DEPTH_MAX);
}
