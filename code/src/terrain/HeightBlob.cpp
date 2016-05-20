/**
 * @file HeightBlob.cpp
 *
 * @see HeibhtBlob.hpp
 */

#include "../../include/terrain/HeightBlob.hpp"

HeightBlob::HeightBlob(Vertex2D newPos, float newHeight, 
		       float newScale,  Biome newBiome) :
    position{newPos}, height{newHeight},
    scale{newScale},  biome{newBiome}
{}




Vertex2D HeightBlob::getPosition() {
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
