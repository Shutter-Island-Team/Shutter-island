/**
 * @file Biome.cpp
 *
 * @see Biome.hpp
 */

#include <iostream>

#include "../../include/Utils.hpp"
#include "../../include/terrain/Biome.hpp"



float biomeHeight(MapParameters& parameters, Biome biome) {

    // Simply generating a random height depending on the biome
    switch (biome) {
    	
    case Sea :
	return random(parameters.getHeightMinSea(), parameters.getHeightMaxSea());
	break;

    case Lake :
	return random(parameters.getHeightMinLake(), parameters.getHeightMaxLake());
	break;

    case Beach :
	return random(parameters.getHeightMinBeach(), parameters.getHeightMaxBeach());
	break;

    case Plains :
	return random(parameters.getHeightMinPlains(), parameters.getHeightMaxPlains());
	break;

    case Mountain :
	return random(parameters.getHeightMinMountain(), parameters.getHeightMaxMountain());
	break;

    case Peak :
	return random(parameters.getHeightMinPeak(), parameters.getHeightMaxPeak());
	break;
	
    case Undefined :
    default:
	std::cerr << "Error : Trying to get the height of an undefined biome" << std::endl;
	exit(1);

    }
}



bool checkSubdivision(MapParameters& parameters, int currentDepth) {
    return (currentDepth < parameters.getBiomeDepthMax());
}
