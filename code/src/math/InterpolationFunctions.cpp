/**
 * @file InterpolationFunctions
 *
 * @see InterpolationFunctions.hpp
 */


#include "../../include/math/InterpolationFunctions.hpp"


float linearInterpolation(float x, float size) {
    
    if (x >= size) {
	return 0.f;
    } else {
	return (1.0f - x/size);
    }
}

float smooth6Interpolation(float x, float size) {
    
    if (x >= size) {
	return 0.f;
    } else {
	float size3 = size*size*size;
	float size4 = size3*size;
	float size5 = size4*size;

	float a = - 6.0f/size5;
	float b =  15.0f/size4;
	float c = -10.0f/size3;
	
	return 1.0f + x*(x*(x*(c + x*(b + x*a))));
    }
}
