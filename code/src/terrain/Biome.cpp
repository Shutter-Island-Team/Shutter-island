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



bool checkSubdivision(int currentDepth) {
    return (currentDepth <= BIOME_DEPTH_MAX);
}
