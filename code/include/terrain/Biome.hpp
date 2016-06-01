/**
 * @file Biome.hpp
 *
 * @brief Everything related to a biome definition
 */
#ifndef BIOME_HPP
#define BIOME_HPP

#include "../../include/terrain/MapParameters.hpp"

/**
 * @brief
 * Defines the minimal height associated with a biome of sea
 */
#define HEIGHT_MIN_SEA     (-  2.0f)

/**
 * @brief
 * Defines the maximal height associated with a biome of sea 
 */
#define HEIGHT_MAX_SEA     (-  0.5f)

/**
 * @brief
 * Defines the minimal height associated with a biome of lake
 * The value is more important than the sea to make sure it carves the land
 */
#define HEIGHT_MIN_LAKE    (-  2.0f)

/**
 * @brief
 * Defines the maximal height associated with a biome of lake
 * The value is more important than the sea to make sure it carves the land
 */
#define HEIGHT_MAX_LAKE    (-  1.0f)


/**
 * @brief
 * Defines the minimal height associated with a biome "Beach".
 */
#define HEIGHT_MIN_BEACH   (   1.0f)

/**
 * @brief
 * Defines the maximal height associated with a biome "Beach".
 */
#define HEIGHT_MAX_BEACH   (   2.0f)
/**
 * @brief
 * Defines the minimal height associated with a biome "Plains".
 */
#define HEIGHT_MIN_PLAINS  (   2.0f)

/**
 * @brief
 * Defines the maximal height associated with a biome "Plains".
 */
#define HEIGHT_MAX_PLAINS  (   4.0f)

/**
 * @brief
 * Defines the minimal height associated with a biome "Mountain".
 * (actually a hill)
 */
#define HEIGHT_MIN_MOUTAIN (   5.0f)

/**
 * @brief
 * Defines the maximal height associated with a biome "Mountain".
 * (actually a hill)
 */
#define HEIGHT_MAX_MOUTAIN (   10.0f)

/**
 * @brief
 * Defines the minimal height associated with a biome "Peak".
 */
#define HEIGHT_MIN_PEAK    (  20.0f)

/**
 * @brief
 * Defines the maximal height associated with a biome "Peak".
 */
#define HEIGHT_MAX_PEAK    (  30.0f)

/**
 * @brief
 * A scale parameter to limit the inluence of mountains on other biomes
 */
#define SCALE_MOUNTAIN     (   0.05f)



/**
 * @brief
 * Defines the minimal number of recursive subdivisions in the QuadTree used to
 * generate the HeightMap.
 */
#define BIOME_DEPTH_MIN    (2)

/**
 * @brief
 * Defines the maximal number of recursive subdivisions in the QuadTree used to
 * generate the HeightMap.
 */
#define BIOME_DEPTH_MAX    (8)



/**
 * @brief This enum defines the biomes that
 * will be used on the map.
 */
enum Biome {
    Undefined, 
    Lake,
    Sea, 
    Beach, 
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
