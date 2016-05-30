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
    	
    case Sea :
	return random(HEIGHT_MIN_SEA,     HEIGHT_MAX_SEA);
	break;

    case Lake :
	return random(HEIGHT_MIN_LAKE,    HEIGHT_MAX_LAKE);
	break;

    case Beach :
	return random(HEIGHT_MIN_BEACH,   HEIGHT_MAX_BEACH);
	break;

    case Plains :
	return random(HEIGHT_MIN_PLAINS,  HEIGHT_MAX_PLAINS);
	break;

    case Mountain :
	return random(HEIGHT_MIN_MOUTAIN, HEIGHT_MAX_MOUTAIN);
	break;

    case Peak :
	return random(HEIGHT_MIN_PEAK,    HEIGHT_MAX_PEAK);
	break;
	
    case Undefined :
    default:
	std::cerr << "Error : Trying to get the height of an undefined biome" << std::endl;
	exit(1);

    }
}



bool checkSubdivision(int currentDepth) {
    return (currentDepth < BIOME_DEPTH_MAX);
}
