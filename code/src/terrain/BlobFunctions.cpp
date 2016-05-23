/**
 * @file BlobFunctions
 *
 * @see BlobFunctions.hpp
 */


#include "../../include/terrain/BlobFunctions.hpp"


float blobSharp(float x, float size, float scale) {
    
    float blobSize = scale * size;
    
    if (x >= blobSize) {
	return 0.f;
    } else {
	return (1.0f - x/blobSize);
    }
}

float blobSmooth6(float x, float size, float scale) {
    
    float blobSize = scale * size;

    if (x >= blobSize) {
	return 0.f;
    } else {
	float blobSize3 = blobSize*blobSize*blobSize;
	float blobSize4 = blobSize3*blobSize;
	float blobSize5 = blobSize4*blobSize;

	float a = - 6.0f/blobSize5;
	float b =  15.0f/blobSize4;
	float c = -10.0f/blobSize3;
	
	return 1.0f + x*(x*(x*(c + x*(b + x*a))));
    }
}
