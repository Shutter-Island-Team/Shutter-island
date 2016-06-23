/**
 * @file Biome.cpp
 *
 * @see Biome.hpp
 */

#include <iostream>
#include <string.h>

#include "../../include/Utils.hpp"
#include "../../include/terrain/Biome.hpp"

const char* biomeToString(Biome biome)
{
	switch (biome) {
		case Undefined:
			return "Undefined";
			break;
		case Lake:
			return "Lake";
			break;
		case Sea:
			return "Sea";
			break;
		case InnerBeach:
			return "InnerBeach";
			break;
		case OuterBeach:
			return "OuterBeach";
			break;
		case Plains:
			return "Plains";
			break;
		case Mountain:
			return "Mountain";
			break;
		case Peak:
			return "Peak";
			break;
		default:
			std::cerr << "biomeToString - Wrong biome type." << std::endl;
			exit(EXIT_FAILURE);
			break;
	}
}

Biome stringToBiome(const char* biomeString)
{
	if (strcmp(biomeString, "Undefined") == 0) {
		return Undefined;
	} else if (strcmp(biomeString, "Lake") == 0) {
		return Lake;
	} else if (strcmp(biomeString, "Sea") == 0) {
		return Sea;
	} else if (strcmp(biomeString, "InnerBeach") == 0) {
		return InnerBeach;
	} else if (strcmp(biomeString, "OuterBeach") == 0) {
		return OuterBeach;
	} else if (strcmp(biomeString, "Plains") == 0) {
		return Plains;
	} else if (strcmp(biomeString, "Mountain") == 0) {
		return Mountain;
	} else if (strcmp(biomeString, "Peak") == 0) {
		return Peak;
	} else {
		std::cerr << "biomeToString - Wrong biome type." << std::endl;
		exit(EXIT_FAILURE);
	}
}

float biomeHeight(MapParameters& parameters, Biome biome) {

    // Simply generating a random height depending on the biome
    switch (biome) {
    	
    case Sea :
	return random(parameters.getHeightMinSea(), parameters.getHeightMaxSea());
	break;

    case Lake :
	return random(parameters.getHeightMinLake(), parameters.getHeightMaxLake());
	break;

    case InnerBeach :
	return random(parameters.getHeightMinInnerBeach(), parameters.getHeightMaxInnerBeach());

    case OuterBeach :
	return random(parameters.getHeightMinOuterBeach(), parameters.getHeightMaxOuterBeach());
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
