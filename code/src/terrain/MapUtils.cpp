/**
 * @file MapUtils.cpp
 *
 * @see MapUtils.hpp
 */

#include <cmath>
#include <iostream>

#include "../../include/terrain/MapUtils.hpp"


// Internal function to get the closest cell
// ie the cell the position is in
int findClosestCell(Vertex2D & pos, 
		    voro::container & container)
{
    double rx, ry, rz;
    int seedID;
    std::cout << container.total_particles() << std::endl;
    if (
            !(
                container.find_voronoi_cell(
                    (double) pos.first, 
                    (double) pos.second, 
                    0.0,
                    rx, 
                    ry, 
                    rz, 
                    seedID
                )
            )
    ) {
        std::cerr << "Cell not found." << std::endl;
        exit(EXIT_FAILURE);
    } 
    return seedID;
}

Biome findClosestBiome(Vertex2D & pos, 
		       voro::container & container, 
		       std::vector<Seed> & seeds) 
{
    int cellId = findClosestCell(pos, container);
    return seeds[cellId].getBiome();
}

void findClosestCentroid(Vertex2D & pos, 
			 voro::container & container, 
			 std::vector<Seed> & seeds,
			 float & xCentroid, float & yCentroid) 
{
    int cellId = findClosestCell(pos, container);
    xCentroid = seeds[cellId].getCentroidX();
    yCentroid = seeds[cellId].getCentroidY();
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
