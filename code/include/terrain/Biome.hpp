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
    OuterBeach, 
    Plains, 
    Mountain,
    Peak
};

/**
 * @brief
 * Converts a biome type to its string representation.
 *
 * @param biome The biome type to convert.
 * @return const char* The string representation of the considered biome type.
 */
const char* biomeToString(Biome biome);

/**
* @brief
* Converts a string representation of a biome into the matching biome type.
*
* @param biomeString The string representation to convert.
* @return Biome The biome type associated with the string representation.
*/
Biome stringToBiome(const char* biomeString);

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
