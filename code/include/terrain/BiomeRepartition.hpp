/**
 * @file BiomeRepartition.hpp
 *
 * @brief Set of functions to attribute biomes to a set of seeds
 */


#include "VoronoiSeedsGenerator.hpp"

/**
 * @brief Repartition of the seeds between land and seas
 * 
 * @param seeds   Set of seeds
 * @param mapSize The size of the map
 * This set must be sorted by the distance to the center of the map
 */
void computeLand(std::vector<Seed>& seeds, float mapSize);



/**
 * @brief Split the water biomes in sea and lakes, also create the beachs
 * 
 * @param seeds Set of seeds
 * This set must be sorted by the distance to the center of the map
 */
void splitWater(std::vector<Seed>& seeds, float mapSize);


/**
 * @brief Raise the moutains on the land
 *
 * @param seeds Set of seeds
 * @param p     Parameter of the geometric law (between 0 and 1) to control
 *              the number of mountains (~1/p)
 * This set must be sorted by the distance to the center of the map
 */

void raiseMountains(std::vector<Seed>& seeds, float p);
