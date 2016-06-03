#include <algorithm>
#include <iostream>

#include "../../include/boids2D/BoidsManager.hpp"

BoidsManager::BoidsManager(MapGenerator& map) 
	: m_map(map)
{
	m_movableBoids = std::make_shared<Matrix<MovableBoidPtr> >(25, 25);
	m_rootedBoids = std::make_shared<Matrix<RootedBoidPtr> >(25, 25);
}

BoidsManager::~BoidsManager()
{

}

MovableBoidPtr BoidsManager::addMovableBoid(BoidType boidType, glm::vec3 location, glm::vec3 velocity) 
{
	MovableBoidPtr movableBoid;
	MovableParametersPtr parameters = std::make_shared<MovableParameters>(boidType);

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
	coordToBox(location, i, j);
    m_movableBoids->add(i, j, movableBoid);
    
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
	int i;
	int j;
	coordToBox(location, i, j);
    m_rootedBoids->add(i, j, rootedBoid);
	
	return rootedBoid;
}

const std::vector<MovableBoidPtr> BoidsManager::getMovableBoids() const
{
	std::vector<MovableBoidPtr> v;
	for (int i = 0; i < m_movableBoids->getNumLine(); ++i) {
		for (int j = 0; j < m_movableBoids->getNumCol(); ++j) {
			for (std::list<MovableBoidPtr>::const_iterator it = m_movableBoids->at(i,j).begin(); it != m_movableBoids->at(i,j).end(); ++it) {
				v.push_back( *it );
			}
		}
	}
	return v;
}

const MatrixMovableBoidPtr & BoidsManager::getMovableBoidsMatrix() const
{
	return m_movableBoids;
}


const std::vector<RootedBoidPtr> BoidsManager::getRootedBoids() const
{
	std::vector<RootedBoidPtr> v;
	for (int i = 0; i < m_rootedBoids->getNumLine(); ++i) {
		for (int j = 0; j < m_rootedBoids->getNumCol(); ++j) {
			for (std::list<RootedBoidPtr>::const_iterator it = m_rootedBoids->at(i,j).begin(); it != m_rootedBoids->at(i,j).end(); ++it) {
				v.push_back( *it );
			}
		}
	}
	return v;
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
	///< @todo mistake for i or j near border negative ?
	std::vector<MovableBoidPtr> v;
	for (int il = i-1; il <= i+1; ++il) {
		for (int jl = j-1; jl <= j+1; ++jl) {
			for (std::list<MovableBoidPtr>::const_iterator it = m_movableBoids->at(i,j).begin(); it != m_movableBoids->at(i,j).end(); ++it) {
				v.push_back( *it );
			}
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
	std::list<MovableBoidPtr>::iterator itm;
	for (int i = 0; i < m_movableBoids->getNumLine(); ++i) {
		for (int j = 0; j < m_movableBoids->getNumCol(); ++j) {
			itm = m_movableBoids->at(i,j).begin();
			while ( itm != m_movableBoids->at(i,j).end()) {
				if (!((*itm)->isFoodRemaining())) {
					(*itm)->disapear();
					itm = m_movableBoids->at(i,j).erase(itm);
				} else {
					itm++;
				}
			}
		}
	}
	std::list<RootedBoidPtr>::iterator itr;
	for (int i = 0; i < m_rootedBoids->getNumLine(); ++i) {
		for (int j = 0; j < m_rootedBoids->getNumCol(); ++j) {
			itr = m_rootedBoids->at(i,j).begin();
			while ( itr != m_rootedBoids->at(i,j).end()) {
				if (!((*itr)->isFoodRemaining())) {
					(*itr)->disapear();
					itr = m_rootedBoids->at(i,j).erase(itr);
				} else {
					itr++;
				}
			}
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

void BoidsManager::coordToBox(const glm::vec3 & location, int & i, int & j) const
{
	///< @todo : Mistake ?
	i = floor(location.x / 20.0f);
	j = floor(location.y / 20.0f);
}

void BoidsManager::updateBoid(MovableBoidPtr mvB, const glm::vec3 & newPosition, const int & iprev, const int & jprev)
{
	int inext = 0;
	int jnext = 0;
	coordToBox(newPosition, inext, jnext);
	if (iprev != inext || jprev != jnext) {
		m_movableBoids->move(mvB, iprev, jprev, inext, jnext);
	}
}

