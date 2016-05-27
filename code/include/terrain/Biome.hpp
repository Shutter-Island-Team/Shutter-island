/**
 * @file Biome.hpp
 *
 * @brief Everything related to a biome definition
 */
#ifndef BIOME_HPP
#define BIOME_HPP

/**
 * @brief
 * Defines the minimal height associated with a biome of water (namely "Lake"
 * and "Sea").
 */
#define HEIGHT_MIN_WATER   (- 15.0f)

/**
 * @brief
 * Defines the maximal height associated with a biome of water (namely "Lake"
 * and "Sea").
 */
#define HEIGHT_MAX_WATER   (-  2.0f)

/**
 * @brief
 * Defines the minimal height associated with a biome "Beach".
 */
#define HEIGHT_MIN_BEACH   (   5.0f)

/**
 * @brief
 * Defines the maximal height associated with a biome "Beach".
 */
#define HEIGHT_MAX_BEACH   (  10.0f)
/**
 * @brief
 * Defines the minimal height associated with a biome "Plains".
 */
#define HEIGHT_MIN_PLAINS  (  10.0f)

/**
 * @brief
 * Defines the maximal height associated with a biome "Plains".
 */
#define HEIGHT_MAX_PLAINS  (  20.0f)

/**
 * @brief
 * Defines the minimal height associated with a biome "Mountain".
 */
#define HEIGHT_MIN_MOUTAIN (  20.0f)

/**
 * @brief
 * Defines the maximal height associated with a biome "Mountain".
 */
#define HEIGHT_MAX_MOUTAIN (  80.0f)

/**
 * @brief
 * Define the range of a blob influence within its quad, for a close range blob.
 */
#define BLOB_SCALE_SMALL  (0.2f)

/**
 * @brief
 * Define the range of a blob influence within its quad, for a medium range 
 * blob.
 */
#define BLOB_SCALE_MEDIUM (0.8f)

/**
 * @brief
 * Define the range of a blob influence within its quad, for a large range blob.
 */
#define BLOB_SCALE_LARGE  (1.0f)

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
    Mountain
};

/**
 * @brief Pick the height of a biome
 *
 * @param biome 
 * @return The height of the biome
 */

float biomeHeight(Biome biome);


/**
 * @brief Compute if the square defined by its four biomes should be subdivised or not
 *
 * @param currentDepth
 * @return Whether there must be a subdivision or not
 */

bool checkSubdivision(int currentDepth);

#endif
