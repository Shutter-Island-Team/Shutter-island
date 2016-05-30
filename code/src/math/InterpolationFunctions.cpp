/**
 * @file InterpolationFunctions
 *
 * @see InterpolationFunctions.hpp
 */


#include "../../include/math/InterpolationFunctions.hpp"


float linearInterpolation(float x, float size, float scale) {
    
    float localSize = scale * size;

    if (x >= localSize) {
	return 0.f;
    } else {
	return (1.0f - x/localSize);
    }
}

float smooth6Interpolation(float x, float size, float scale) {
    
    float localSize = scale * size;

    if (x >= localSize) {
	return 0.f;
    } else {
	float localSize3 = localSize*localSize*localSize;
	float localSize4 = localSize3*localSize;
	float localSize5 = localSize4*localSize;

	float a = - 6.0f/localSize5;
	float b =  15.0f/localSize4;
	float c = -10.0f/localSize3;
	
	return 1.0f + x*(x*(x*(c + x*(b + x*a))));
    }
}
