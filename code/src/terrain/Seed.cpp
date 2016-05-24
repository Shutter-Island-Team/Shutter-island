/**
 * @file Seed.cpp
 *
 * @see Seed.hpp
 */


#include "../../include/terrain/Seed.hpp"


Seed::Seed(float x, float y, Biome biome):
    cell{NULL},
    position{Vertex2D(x, y)},
    biomeType{biome}
{}

Seed::Seed(const Seed& seed)
    :   cell{NULL},
  position{Vertex2D(seed.position.first, seed.position.second)},
        biomeType{seed.biomeType}
{
    this->setCell(seed.getCell());
}
        

void Seed::setX(float newX)
{
    this->position.first = newX;
}

float Seed::getX() const
{
    return this->position.first;
}



void Seed::setY(float newY)
{
    this->position.second = newY;
}

float Seed::getY() const
{
    return this->position.second;
}




void Seed::setBiome(Biome newBiome)
{
    this->biomeType = newBiome;
}

Biome Seed::getBiome() const
{
    return this->biomeType;
}




void Seed::setCell(voroNeighborPtr newCell) 
{   
    this->cell = newCell;
}

voroNeighborPtr Seed::getCell() const
{
    return this->cell;
}




Seed& Seed::operator=(const Seed& seed)
{
    if (this != &seed) {
        this->setCell(seed.getCell());
        this->setX(seed.getX());
        this->setY(seed.getY());
        biomeType = seed.biomeType;
    }

    return *this;
}


Seed::~Seed() 
{}
