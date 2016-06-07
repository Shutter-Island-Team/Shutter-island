/**
 * @file MapUtils.cpp
 *
 * @see MapUtils.hpp
 */

#include <cmath>
#include <iostream>

#include "../../include/terrain/MapUtils.hpp"
#include "../../include/math/InterpolationFunctions.hpp"
/**
 * @brief
 * Macro aiming at computing an image of the distance between two points.
 * 
 * @param x0 The abscissa of the first point.
 * @param y0 The ordinate of the first point.
 * @param x1 The abscissa of the second point.
 * @param y1 The ordinate of the second point.
 */
#define DIST_2(x0,y0,x1,y1) ((x0)-(x1))*((x0)-(x1))+((y0)-(y1))*((y0)-(y1))

/**
 * @brief Macro aiming to correct the negative values that approximate 0
 * returned by the interpolation functions
 *
 * @param The value to correct
 */ 
#define CORRECT_ERROR(v) (((v)<0)?(0.0f):(v))

// Internal function to get the closest cell
// ie the cell the position is in
int findClosestCell(
		    Vertex2D& pos, 
		    voro::container& container
		    )
{
    double rx, ry, rz;
    int seedID;
    // Calling the voro++ function
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
        std::cerr << "(Requested position : (" << pos.first
		  << ", " << pos.second << "))"<< std::endl;
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



Biome findApproximativeBiome(Vertex2D & pos,
			     int effMapSize,
			     Biome* biomeMap,
			     int mapScaling) {
    // Simply taking the closest lower bound value
    int closeI = (int) pos.first*mapScaling;
    int closeJ = (int) pos.second*mapScaling;

    return biomeMap[closeI+closeJ*effMapSize];
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

bool findClosestLake(
		     std::vector<glm::vec2>& lakes,
		     float x,
		     float y,
		     float& xLake,
		     float& yLake
		     )
{
    /*
     * If the vector is empty, then, there is no lake on the map !
     */
    if (lakes.size() != 0) {
	/*
	 * Preparation of the loop on the Lake biome in order to determine which Lake
	 * biome is the closest of the given coordinates.
	 */
	auto lakesIt = lakes.begin();
	float minDist = DIST_2(x, y, lakesIt->x, lakesIt->y);
	xLake = lakesIt->x;
	yLake = lakesIt->y;
	lakesIt++;

	for (; lakesIt != lakes.end(); lakesIt++)
	    {
		float currentDist = DIST_2(x, y, lakesIt->x, lakesIt->y);
		if (currentDist < minDist) {
		    minDist = currentDist;
		    xLake = lakesIt->x;
		    yLake = lakesIt->y;
		}
	    }
	return true;
    } else {
	return false;
    }
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


// See the hpp to have a detailed explanation of the following functions
float computeInterpolationCoefficient(MapParameters & mapParameters, 
				      Biome biome1, Biome biome2,
				      float x,      float xMax) {

    float result;

    if ((biome1 == Peak) || (biome1 == Mountain)) {
	if ((biome2 == Peak) || (biome2 == Mountain)) {
	    // Case A
	    result = linearInterpolation(x, xMax);
	} else {
	    // Case B
	    result = smooth6Interpolation(x, xMax, 
					  mapParameters.getScaleLimitInfluence());
	}
    } else {
	if ((biome2 == Peak) || (biome2 == Mountain)) {
	    // Case B
	    result = 1.0f - smooth6Interpolation(xMax - x, xMax, 
					  mapParameters.getScaleLimitInfluence());
	} else {
	    if (biome1 == Sea) {
		if (biome2 == Sea) {
		    // Case C
		    result = smooth6Interpolation(x, xMax);
		} else {
		    // Case D
		    result = smooth6Interpolation(x, xMax, 
						  mapParameters.getScaleLimitInfluence());
		}
	    } else {
		if (biome2 == Sea) {
		    // Case D
		    result = 1.0f - smooth6Interpolation(xMax - x, xMax, 
						  mapParameters.getScaleLimitInfluence());
		} else {
		    // Case E
		    result = smooth6Interpolation(x, xMax);
		}
	    }
	}
    }
    return CORRECT_ERROR(result);
}

					 
void countBiome(Biome biome,        int scale,
		int *seaCount,      int *sandCount,
		int *plainsCount,   int *lakeCount,
		int *mountainCount, int *peakCount) {
    
    switch(biome) {
    
	
    case Sea:
	*seaCount += scale;
	break;

    case InnerBeach:
    case OuterBeach:
	*sandCount += scale;
	break;

    case Plains:
	*plainsCount += scale;
	break;

    case Lake:
	*lakeCount += scale;
	break;

    case Mountain:
	*mountainCount += scale;
	break;

    case Peak:
	*peakCount += scale;
	break;

    default:
	throw std::invalid_argument("Wrong biome (countBiome)");
	break;
    }
}
