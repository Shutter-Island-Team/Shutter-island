#include <algorithm>
#include <iostream>

#include "../../include/boids2D/BoidsManager.hpp"

BoidsManager::BoidsManager(MapGenerator& map) 
	: m_map(map)
{

}

BoidsManager::~BoidsManager()
{

}

MovableBoidPtr BoidsManager::addMovableBoid(BoidType boidType, glm::vec3 location, glm::vec3 velocity) 
{
	MovableBoidPtr movableBoid;
	MovableParameters *parameters = new MovableParameters(boidType);

	switch(boidType)
	{
		case WOLF:
			movableBoid = std::make_shared<MovableBoid>(location, velocity, WOLF, parameters);
			break;
		case RABBIT:
			movableBoid = std::make_shared<MovableBoid>(location, velocity, RABBIT, parameters);
			break;
		default:
			throw std::invalid_argument("valid boidType required");
			break;
	}
    m_movableBoids.push_back(movableBoid);
    
    return movableBoid;
}

RootedBoidPtr BoidsManager::addRootedBoid(BoidType boidType, glm::vec3 location)
{
	RootedBoidPtr rootedBoid;
	switch(boidType)
	{
		case CARROT:
			rootedBoid = std::make_shared<RootedBoid>(location, CARROT);
			break;
		case TREE:
			rootedBoid = std::make_shared<RootedBoid>(location, TREE);
			break;
		default:
			throw std::invalid_argument("valid boidType required");
			break;
	}
	m_rootedBoids.push_back(rootedBoid);
	
	return rootedBoid;
}


const std::vector<MovableBoidPtr>& BoidsManager::getMovableBoids() const
{
	return m_movableBoids;
}

const std::vector<RootedBoidPtr>& BoidsManager::getRootedBoids() const
{
	return m_rootedBoids;
}

bool BoidsManager::isNight() const
{
	return isNightTime;
}

void BoidsManager::setTimeDay(bool state)
{
	isNightTime = state;
}

const std::vector<MovableBoidPtr> & BoidsManager::getNeighbour(MovableBoid mvB) const
{
	return m_movableBoids;
}

Biome BoidsManager::getBiome(const float& x, const float& y) const
{
	return m_map.getBiome(x, y);
}
    

float BoidsManager::getHeight(const float& x, const float& y) const
{
	return m_map.getHeight(x, y);
}


MapGenerator& BoidsManager::getMap() const
{
	return m_map;
}

void BoidsManager::removeDead()
{
	std::vector<MovableBoidPtr>::iterator it = m_movableBoids.begin();
	while ( it != m_movableBoids.end()) {
		if (!((*it)->isFoodRemaining())) {
			(*it)->disapear();
			it = m_movableBoids.erase(it);
		} else {
			it++;
		}
	}
}