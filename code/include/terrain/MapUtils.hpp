/**
 * @file MapUtils.hpp
 *
 * @brief Some useful functions to compute the height map
 */

#ifndef MAPUTILS_HPP
#define MAPUTILS_HPP


#include "../../lib/voro++/src/voro++.hh"
#include "VoronoiSeedsGenerator.hpp"
#include "MapParameters.hpp"

#include <glm/glm.hpp>

/**
 * @brief Find the centroid of the biome associated to a position
 *
 * @param pos   The desired position.
 * @param seeds The vector containing the seeds used to generate the Voronoi
 * diagram.
 * @param container The container of the "voronoicell" defined by the library
 * "voro++" and reprensenting a cell of the Voronoi diagram.
 *
 * @param xCentroid The abcissa of the centroid
 * @param yCentroid The ordinna of the centroid
 */

void findClosestCentroid(Vertex2D & pos, 
			 voro::container & container, 
			 std::vector<Seed> & seeds,
			 float & xCentroid, float & yCentroid) ;


/**
 * @brief Find the biome associated to a position
 *
 * @param pos   The desired position.
 * @param seeds The vector containing the seeds used to generate the Voronoi
 * diagram.
 * @param container The container of the "voronoicell" defined by the library
 * "voro++" and reprensenting a cell of the Voronoi diagram.
 *
 * @return The biome of pos
 */

Biome findClosestBiome(Vertex2D & pos, 
		       voro::container & container, 
		       std::vector<Seed> & seeds);

/**
 * @brief
 * Determines which Lake biome is the closest of the given position.
 *
 * @param lakes A reference on the vector containing the positions of the Lake
 * biome.
 * @param x The abscissa of the point this function aims at dertermining the
 * closest Lake biome.
 * @param y The ordinate of the point this function aims at dertermining the
 * closest Lake biome.
 * @param xLake A reference on a float in which the function is going to store
 * the abscissa of the centroid of the closest Lake biome.
 * @param yLake A reference on a float in which the function is going to store
 * the ordinate of the centroid of the closest Lake biome.
 *
 * @return A boolean representing "Closest Lake biome found ?"
 */
bool findClosestLake(
	std::vector<glm::vec2>& lakes,
	float x,
	float y,
	float& xLake,
	float& yLake
);

/**
 * @brief Compute the distance between two Vertex2D
 *
 * @param a The first vertex
 * @param b The second vertex
 *
 * @return The distance between a and b
 */
float distanceV2D(Vertex2D & a, Vertex2D & b);

/**
 * @brief Compute an interpolation coefficient depending
 * on the biomes
 *
 *  a) If both vertices belong to a mountain area (Mountain or Peak),
 *       we can assume that the edge belongs to a mountain are and do a
 *       sharp (linear) interpolation
 *       Seas are also added to this category as the effect will not be seen underwater
 *       and as it helps in step c)
 *    b) If both vertices do not belong to a mountain or sea area, to acknoledge
 *       the border, we do a smooth interpolation.
 *    c) Otherwise, there is a mountain or a sea-hole and a normal land. We thus do a smooth
 *       interpolation to acknoledge the transition between the two biomes, but
 *       we control this interpolation with a scale parameter to reduce the area
 *       of influence of the mountain/sea vertex.
 *
 * @param biome1 The first biome
 * @param biome2 The second biome
 * @param x      The distance to the first biome
 * @param xMax   The distance between the two biomes
 *
 * @return The interpolation coefficient
 */

float computeInterpolationCoefficient(MapParameters & mapParameters,
				      Biome biome1, Biome biome2,
				      float x,      float xMax);

#endif
