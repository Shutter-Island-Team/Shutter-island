/**
 * @file Seed.cpp
 *
 * @see Seed.hpp
 */


#include "../../include/terrain/Seed.hpp"


Seed::Seed(float x, float y, Biome biome):
    position{Vertex2D(x, y)},
    biomeType{biome}
{}



void Seed::setX(float newX)
{
    this->position.first = newX;
}

float Seed::getX()
{
    return this->position.first;
}



void Seed::setY(float newY)
{
    this->position.second = newY;
}

float Seed::getY()
{
    return this->position.second;
}




void Seed::setBiome(Biome newBiome)
{
    this->biomeType = newBiome;
}

Biome Seed::getBiome()
{
    return this->biomeType;
}
