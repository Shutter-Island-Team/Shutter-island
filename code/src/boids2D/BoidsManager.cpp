#include <algorithm>
#include <iostream>

#include "../../include/boids2D/BoidsManager.hpp"

BoidsManager::BoidsManager(MapGenerator& map) 
	: m_map(map), m_movableBoids(Matrix<MovableBoidPtr>(5, 5))
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
	int i;
	int j;
	CoordToBox(location, i, j);
    m_movableBoids.push_back(i, j, movableBoid);
    
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

const std::vector<MovableBoidPtr> BoidsManager::getMovableBoids() const
{
	std::vector<MovableBoidPtr> v;
	for (int i = 0; i < m_movableBoids.getNumLine(); ++i) {
		for (int j = 0; j < m_movableBoids.getNumCol(); ++j) {
			v.insert( v.end(), m_movableBoids.at(i,j).begin(), m_movableBoids.at(i,j).end() );
		}
	}
	return v;
}

const Matrix<MovableBoidPtr> BoidsManager::getMovableBoidsMatrix() const
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

const std::vector<MovableBoidPtr> BoidsManager::getNeighbour(MovableBoid mvB, const int & i, const int & j) const
{
	std::cerr << "I hear the call" << std::endl;
	///< @todo mistake for i or j near border negative ?
	std::vector<MovableBoidPtr> v;
	for (int il = i-1; il <= i+1; ++il) {
		for (int jl = j-1; jl <= j+1; ++jl) {

		}
	}
	return v;
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
	for (int i = 0; i < m_movableBoids.getNumLine(); ++i) {
		for (int j = 0; j < m_movableBoids.getNumCol(); ++j) {
			std::vector<MovableBoidPtr>::iterator itm = m_movableBoids.at(i,j).begin();
			while ( itm != m_movableBoids.at(i,j).end()) {
				if (!((*itm)->isFoodRemaining())) {
					(*itm)->disapear();
					itm = m_movableBoids.at(i,j).erase(itm);
				} else {
					itm++;
				}
			}
		}
	}
	std::vector<RootedBoidPtr>::iterator itr = m_rootedBoids.begin();
	while ( itr != m_rootedBoids.end()) {
		if (!((*itr)->isFoodRemaining())) {
			(*itr)->disapear();
			itr = m_rootedBoids.erase(itr);
		} else {
			itr++;
		}
	}
}

bool BoidsManager::getNearestLake(const MovableBoidPtr & boid, glm::vec2 & result) const
{
	return getNearestLake(glm::vec2(boid->getLocation().x, boid->getLocation().y), result);
}

bool BoidsManager::getNearestLake(const glm::vec2 & position, glm::vec2 & result) const
{
	return m_map.getClosestLake(position.x, position.y, result.x, result.y);
}

void BoidsManager::CoordToBox(const glm::vec3 & location, int & i, int & j) const
{
	///< @todo : Mistake ?
	i = floor(location.x / 100.0f);
	j = floor(location.y / 100.0f);
}
