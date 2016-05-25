/**
 * @file MapUtils.cpp
 *
 * @see MapUtils.hpp
 */

#include <cmath>

#include "../../include/terrain/MapUtils.hpp"


Biome findClosestBiome(Vertex2D & pos, 
		       voro::container & container, std::vector<Seed> & seeds){
    double rx, ry, rz;
    int seedID;
    container.find_voronoi_cell(pos.first, pos.second, 0.0,
				rx, ry, rz, seedID);
    Seed closestSeed = seeds[seedID];
    return closestSeed.getBiome();
}


float distanceV2D(Vertex2D & a, Vertex2D & b) {
    
    float aX = a.first;
    float aY = a.second;
    float bX = b.first;
    float bY = b.second;

    float u = bX - aX;
    float v = bY - aY;

    return sqrt(u*u + v*v);
}
