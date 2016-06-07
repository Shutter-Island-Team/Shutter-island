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
 * @brief
 * Min function between two numbers.
 *
 * @param a First number.
 * @param b Second number.
 */
#define MIN(a, b) (((a)<(b))?(a):(b))

/**
 * @brief
 * Max function between two numbers.
 *
 * @param a First number.
 * @param b Second number.
 */
#define MAX(a, b) (((a)>(b))?(a):(b))


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
 * @brief Find the approximative biome 
 * associated to a location using a sampled biome map
 * 
 * @param pos         The desired position
 * @param effMapSize  The size of the sampled map
 * @param biomeMap    The sampled biome map
 * @param mapSacing   The sampling resolution
 *
 * @return A biome close to the position
 */
Biome findApproximativeBiome(Vertex2D & pos,
			     int effMapSize,
			     Biome* biomeMap,
			     int mapScaling);

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
 *  The function implements the following decision function :
 *  * A) The two biomes are from a mountain zone : 
 *       Linear (sharp) interpolation
 *  * B) One of the two biomes is a mountain :
 *       We're on a border between a mountain and another biome
 *       The mountain should not raise the other biome
 *       Smooth interpolation limiting the influence of the mountain
 *  * C) The two biomes are from a sea zone :
 *       Classic smooth interpolation
 *  * D) One of the two biomes is a sea :
 *       We're on a border between a sea and another biome
 *       The sea should not drown the other biome
 *       Smooth interpolation limiting the influence of the sea
 *  * E) Else :
 *       Classic smooth interpolation
 *
 * @param mapParameters The parameters of the map generation
 * @param biome1        The first biome
 * @param biome2        The second biome
 * @param x             The distance to the first biome
 * @param xMax          The distance between the two biomes
 *
 * @return The interpolation coefficient
 */

float computeInterpolationCoefficient(MapParameters & mapParameters,
				      Biome biome1, Biome biome2,
				      float x,      float xMax);

/**
 * @brief Increment a coefficient to count a biome
 *
 * @param biome         The biome
 * @param scale         Scale parameter to see how much a biome counts 
 *                           (1 or -1 typically) 
 * @param seaCount      Number of sea      biomes
 * @param sandCount     Number of sand     biomes
 * @param plainsCount   Number of plains   biomes
 * @param lakeCount     Number of lake     biomes
 * @param mountainCount Number of mountain biomes
 * @param peakCount     Number of peak     biomes
 */
void countBiome(Biome biome,        int scale,
		int *seaCount,      int *sandCount,
		int *plainsCount,   int *lakeCount,
		int *mountainCount, int *peakCount);

#endif
