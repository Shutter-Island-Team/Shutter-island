/**
 * @file HeightBlob.cpp
 *
 * @see HeibhtBlob.hpp
 */

#include <iostream>
#include "../../include/terrain/MapUtils.hpp"
#include "../../include/terrain/BlobFunctions.hpp"
#include "../../include/terrain/HeightBlob.hpp"

HeightBlob::HeightBlob(Vertex2D newPos, float newHeight, 
		       float newScale,  Biome newBiome) :
    position{newPos}, height{newHeight},
    scale{newScale},  biome{newBiome}
{}




Vertex2D& HeightBlob::getPosition() {
    return position;
}


float HeightBlob::getHeight() {
    return height;
}


float HeightBlob::getScale() {
    return scale;
}


Biome HeightBlob::getBiome() {
    return biome;
}


float HeightBlob::evalHeight(Vertex2D pos, float size) {
    
    float dist = distanceV2D(this->position, pos);

    switch (biome) {

    case Undefined :
	std::cerr << "Error : trying to evaluate an undefined blob" << std::endl;
	exit(1);
	
    case Mountain :
	return height*blobSharp(dist, size, scale);

    default :
	return height*blobSmooth6(dist, size, scale);

    }
}
