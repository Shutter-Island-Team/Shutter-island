/**
 * @file MapUtils.hpp
 *
 * @brief Some useful functions to compute the height map
 */

#ifndef MAPUTILS_HPP
#define MAPUTILS_HPP


#include "../../lib/voro++/src/voro++.hh"

#include "VoronoiSeedsGenerator.hpp"


/**
 * @brief Find the biome associated to a position
 *
 * @param pos   The desired position
 * @param seeds The container of the seeds
 *
 * @return The biome of pos
 */

Biome findClosestBiome(Vertex2D & pos, 
		       voro::container & container, std::vector<Seed> & seeds);


/**
 * @brief Compute the distance between two Vertex2D
 *
 * @param a The first vertex
 * @param b The second vertex
 *
 * @return The distance between a and b
 */
float distanceV2D(Vertex2D & a, Vertex2D & b);

#endif
