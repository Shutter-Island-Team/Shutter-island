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

void computeCoast(std::vector<Seed> seeds, float mapSize);






 /**
 * @brief Whittaker phase to attribute the biomes on the seeds
 * that have been defined as a land
 */

// TODO
