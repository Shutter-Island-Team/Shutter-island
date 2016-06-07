#include <algorithm>
#include <iostream>

#include "../../include/boids2D/BoidsManager.hpp"
#include "../../include/boids2D/SightRenderable.hpp"
#include "../../include/boids2D/StateRenderable.hpp"

BoidsManager::BoidsManager(MapGenerator& map, Viewer& viewer, ShaderProgramPtr& shader) 
	: m_map(map), m_viewer(viewer), m_shader(shader), m_updateCoeff(0), m_updatePeriod(10)
{
	m_movableBoids = std::make_shared<Matrix<MovableBoidPtr> >(25, 25);
	m_rootedBoids = std::make_shared<Matrix<RootedBoidPtr> >(25, 25);
}

BoidsManager::~BoidsManager()
{

}

MovableBoidPtr BoidsManager::addMovableBoid(BoidType boidType, glm::vec3 location, glm::vec3 landmarkLocation, glm::vec3 velocity) 
{
	MovableBoidPtr movableBoid;
	MovableParametersPtr parameters = std::make_shared<MovableParameters>(boidType);

	switch(boidType)
	{
		case WOLF:
			movableBoid = std::make_shared<MovableBoid>(location, landmarkLocation, velocity, WOLF, parameters);
			break;
		case RABBIT:
			movableBoid = std::make_shared<MovableBoid>(location, landmarkLocation, velocity, RABBIT, parameters);
			break;
		default:
			throw std::invalid_argument("valid boidType required");
			break;
	}
	unsigned int i;
	unsigned int j;
	coordToBox(location, i, j);
    m_movableBoids->add(i, j, movableBoid);
    m_movableBoidsVec.push_back(movableBoid);
    
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
	unsigned int i;
	unsigned int j;
	coordToBox(location, i, j);
    m_rootedBoids->add(i, j, rootedBoid);
	
	return rootedBoid;
}

const std::vector<MovableBoidPtr> & BoidsManager::getMovableBoids() const
{
	return m_movableBoidsVec;
}

const MatrixMovableBoidPtr & BoidsManager::getMovableBoidsMatrix() const
{
	return m_movableBoids;
}


const std::list<RootedBoidPtr> BoidsManager::getRootedBoids(const int & i, const int & j) const
{
	return m_rootedBoids->merge(i,j);
}

const std::vector<RootedBoidPtr> BoidsManager::getAllRootedBoids() const
{
	std::vector<RootedBoidPtr> v;
	for (unsigned int i = 0; i < m_rootedBoids->getNumLine(); ++i) {
		for (unsigned int j = 0; j < m_rootedBoids->getNumCol(); ++j) {
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

const std::list<MovableBoidPtr> BoidsManager::getNeighbour(const int & i, const int & j) const
{
	return m_movableBoids->merge(i,j);
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
	for (unsigned int i = 0; i < m_movableBoids->getNumLine(); ++i) {
		for (unsigned int j = 0; j < m_movableBoids->getNumCol(); ++j) {
			itm = m_movableBoids->at(i,j).begin();
			while ( itm != m_movableBoids->at(i,j).end()) {
				if (!((*itm)->isFoodRemaining()) || (*itm)->isDecomposed()) {
					(*itm)->disapear();
					itm = m_movableBoids->at(i,j).erase(itm);
				} else {
					itm++;
				}
			}
		}
	}

	std::vector<std::vector<MovableBoidPtr>::iterator> toDelete;
	for (std::vector<MovableBoidPtr>::iterator i = m_movableBoidsVec.begin(); i != m_movableBoidsVec.end(); ++i)
	{
		if(!((*i)->isFoodRemaining()) || (*i)->isDecomposed()) {
			(*i)->disapear();
			toDelete.push_back(i);
		}
	}

	for (std::vector<std::vector<MovableBoidPtr>::iterator >::iterator it = toDelete.begin(); it != toDelete.end(); ++it)
	{
		m_movableBoidsVec.erase(*it);
	}
	toDelete.clear();

	std::list<RootedBoidPtr>::iterator itr;
	for (unsigned int i = 0; i < m_rootedBoids->getNumLine(); ++i) {
		for (unsigned int j = 0; j < m_rootedBoids->getNumCol(); ++j) {
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

void BoidsManager::coordToBox(const glm::vec3 & location, unsigned int & i, unsigned int & j) const
{
	///< @todo : Mistake ?
	i = (unsigned int) floor(location.x / 20.0f);
	j = (unsigned int) floor(location.y / 20.0f);
}

void BoidsManager::updateBoidInGrid(MovableBoidPtr mvB, const unsigned int & iprev, const unsigned int & jprev)
{
	unsigned int inext = 0;
	unsigned int jnext = 0;
	coordToBox(mvB->getLocation(), inext, jnext);
	if (iprev != inext || jprev != jnext) {
		m_movableBoids->move(mvB, iprev, jprev, inext, jnext);
	}
}

void BoidsManager::updateTick()
{
	m_updateCoeff++;
}

bool BoidsManager::isUpdateTick() const
{
	return m_updateCoeff == m_updatePeriod;
}

void BoidsManager::resetTick()
{
	m_updateCoeff = 0;
}

void BoidsManager::addDebugMovableBoid(MovableBoidPtr m)
{
    SightRenderablePtr sight = std::make_shared<SightRenderable>(m_shader, m);
    m_viewer.addRenderable(sight);

    StateRenderablePtr state = std::make_shared<StateRenderable>(m_shader, m);
    m_viewer.addRenderable(state);
}
