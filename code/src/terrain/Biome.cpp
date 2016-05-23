/**
 * @file Biome.cpp
 *
 * @see Biome.hpp
 */

#include <iostream>

#include "../../include/Utils.hpp"
#include "../../include/terrai/Biome.hpp"



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



bool checkSubdibision(int currentDepth,
		      biome biomeTL, biome biomeTR,
		      biome biomeBL, biome biomeBR) {

    return ( // Not going to deep in the tree
	    (currentDepth <= BIOME_DEPTH_MAX)
	    && ( // But still deep enough
		(profondeur <= BIOME_DEPTH_MIN)
		// Or if the we are not in a homogeneous area
		|| (not ((biomeTL == biomeTR) && (biomeTR == biomeBL) && (biomeBL == biomeBR)))
		// Or if we are, go only for the detail depth of the area type
		|| (((biomeTL == Sea) || (biomeTL == Beach) || (biomeTL == Lake)) 
		    && (currentDepth <= BIOME_DEPTH_FLAT))
		|| ((biomeTL == Plains)                     
		    && (currentDepth <= BIOME_DEPTH_MEDIUM))
		|| ((biomeTL == Mountain)
		    && (currentDepth <= BIOME_DEPTH_SHARP))));
}
