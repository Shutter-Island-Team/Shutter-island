/**
 * @file HeightData.cpp
 *
 * @see HeightData.hpp
 */

#include <iostream>
#include "../../include/terrain/MapUtils.hpp"
#include "../../include/terrain/HeightData.hpp"

HeightData::HeightData(Vertex2D newPos, float newHeight, Biome newBiome) :
    position{newPos}, height{newHeight}, biome{newBiome}
{}




Vertex2D& HeightData::getPosition() {
    return position;
}


float HeightData::getHeight() {
    return height;
}



Biome HeightData::getBiome() {
    return biome;
}

