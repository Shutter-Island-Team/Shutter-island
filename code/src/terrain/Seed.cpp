/**
 * @file Seed.cpp
 *
 * @see Seed.hpp
 */


#include "../../include/terrain/Seed.hpp"


Seed::Seed(float x, float y, Biome biome):
    cell{NULL},
    position{Vertex2D(x, y)},
    centroid{Vertex2D(-1.0, -1.0)},
    biomeType{biome}
{}

Seed::Seed(const Seed& seed)
    :   cell{NULL},
        position{Vertex2D(seed.position.first, seed.position.second)},
        centroid{Vertex2D(seed.centroid.first, seed.centroid.second)},
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

void Seed::setCentroidX(float newX)
{
    this->centroid.first = newX;
}

float Seed::getCentroidX() const
{
    return this->centroid.first;
}

void Seed::setCentroidY(float newY)
{
    this->centroid.second = newY;
}

float Seed::getCentroidY() const
{
    return this->centroid.second;
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
        this->setCentroidX(seed.getCentroidX());
        this->setCentroidY(seed.getCentroidY());
        biomeType = seed.biomeType;
    }

    return *this;
}

Seed::~Seed() 
{}
