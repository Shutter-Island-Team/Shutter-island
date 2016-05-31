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
 * @brief Add lakes within the lands
 *
 * @param seeds   Set of seeds
 * This set must be sorted by the distance to the center of the map
 * @param lakes A reference on the vector to fill with the coordinates of the
 * centroids of the Lake biomes.
 */
void computeLake(std::vector<Seed>& seeds, std::vector<glm::vec2>& lakes);


/**
 * @brief Transform the plains that touch the sea into beach
 * 
 * @param seeds Set of seeds
 * @param mapSize The size of the map to generate.
 * This set must be sorted by the distance to the center of the map
 */
void computeBeach(std::vector<Seed>& seeds, float mapSize);


/**
 * @brief Raise the moutains on the land
 *
 * @param seeds Set of seeds
 * This set must be sorted by the distance to the center of the map
 */

void computeMountains(std::vector<Seed>& seeds);
