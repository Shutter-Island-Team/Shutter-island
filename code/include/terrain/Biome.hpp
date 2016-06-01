/**
 * @file Biome.hpp
 *
 * @brief Everything related to a biome definition
 */
#ifndef BIOME_HPP
#define BIOME_HPP

#include "../../include/terrain/MapParameters.hpp"

/**
 * @brief This enum defines the biomes that
 * will be used on the map.
 */
enum Biome {
    Undefined, 
    Lake,
    Sea, 
    InnerBeach,
    OutterBeach, 
    Plains, 
    Mountain,
    Peak
};

/**
 * @brief Pick the height of a biome.
 *
 * @param biome The type of biome the height has to be picked.
 * @param parameters The map generation parameters.
 * @return The height of the biome.
 */

float biomeHeight(MapParameters& parameters, Biome biome);


/**
 * @brief 
 * Compute if the square defined by its four biomes should be subdivised or not.
 *
 * @param currentDepth The current depth in the QuadTree/HeightTree.
 * @param parameters The map generation parameters.
 * @return Whether there must be a subdivision or not.
 */

bool checkSubdivision(MapParameters& parameters, int currentDepth);

#endif
