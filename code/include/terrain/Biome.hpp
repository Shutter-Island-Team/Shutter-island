/**
 * @file Biome.hpp
 *
 * @brief Everything related to a biome definition
 */
#ifndef BIOME_HPP
#define BIOME_HPP

#define HEIGHT_MIN_WATER   (- 15.0f)
#define HEIGHT_MAX_WATER   (-  2.0f)
#define HEIGHT_MIN_BEACH   (   5.0f)
#define HEIGHT_MAX_BEACH   (  10.0f)
#define HEIGHT_MIN_PLAINS  (  10.0f)
#define HEIGHT_MAX_PLAINS  (  20.0f)
#define HEIGHT_MIN_MOUTAIN (  20.0f)
#define HEIGHT_MAX_MOUTAIN (  80.0f)

#define BLOB_SCALE_SMALL  (0.2f)
#define BLOB_SCALE_MEDIUM (0.8f)
#define BLOB_SCALE_LARGE  (1.0f)

#define BIOME_DEPTH_MIN    (2)
#define BIOME_DEPTH_MAX    (8)

/**
 * @brief This enum defines the biomes that
 * will be used on the map
 * The 'undefined' biome type should disappear after
 * the Whittaker phase
 */

enum Biome {
    Undefined, 
    Lake,
    Sea, 
    Beach, 
    Plains, 
    Mountain
};

/**
 * @brief Pick the height of a biome
 *
 * @param biome 
 *
 * @return The height of the biome
 */

float biomeHeight(Biome biome);


/**
 * @brief Compute if the square defined by its four biomes should be subdivised or not
 *
 * @param currentDepth
 * @param biomeTL
 * @param biomeTR
 * @param biomeBL
 * @param biomeBR
 *
 * @return Whether there must be a subdivision or not
 */

bool checkSubdivision(int currentDepth);

#endif
