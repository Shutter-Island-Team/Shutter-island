#include <algorithm>
#include <iostream>

#include "../../include/boids2D/BoidsManager.hpp"
#include "../../include/boids2D/SightRenderable.hpp"
#include "../../include/boids2D/StateRenderable.hpp"
#include "../../include/Utils.hpp"

#define MAP_SIZE 500.0
#define NB_RABBIT_MIN 6
#define NB_RABBIT_MAX 10
#define NB_WOLF_MIN 2
#define NB_WOLF_MAX 15
#define NB_TREE_MIN 8
#define NB_TREE_MAX 15
#define NB_CARROT_MIN 8
#define NB_CARROT_MAX 15

BoidsManager::BoidsManager(MapGenerator& map, Viewer& viewer, ShaderProgramPtr& shader) 
	: m_map(map), m_viewer(viewer), m_shader(shader), m_updateCoeff(0), m_updatePeriod(10),
		m_countCarrot(0)
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
			m_countCarrot++;
			rootedBoid = std::make_shared<RootedBoid>(location, CARROT, 0.0f, 1);
			break;
		case TREE:
			rootedBoid = std::make_shared<RootedBoid>(location, TREE, 2.0f, 1);
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
					m_countCarrot--;
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

void BoidsManager::repopCarrot()
{
	float x = random(0, MAP_SIZE);
	float y = random(0, MAP_SIZE);
	glm::vec3 positionFellow(x, y, getHeight(x, y));

    while(getBiome(positionFellow.x, positionFellow.y) != Plains) {
		x = random(0, MAP_SIZE);
		y = random(0, MAP_SIZE);
		positionFellow = glm::vec3(x, y, getHeight(x, y));
    }

	addRootedBoid(CARROT, positionFellow);
}

const int & BoidsManager::getCountCarrot() const
{
	return m_countCarrot;	
}

glm::vec3 BoidsManager::computeBiomeLeaderPosition(Biome biome, float min, float max, float zPos)
{
    glm::vec3 position(random(min, max), random(min, max), zPos);

    while(m_map.getBiome(position.x, position.y) != biome) {
        position = glm::vec3(random(min, max), random(min, max), zPos);
    }

    return position;
}

glm::vec3 BoidsManager::computeBiomeFellowPosition(Biome biome, 
    glm::vec3 position, float min, float max, float zPos, float radius)
{
    glm::vec3 positionFellow(random(min, max), random(min, max), zPos);

    while(glm::length(positionFellow - position) >= radius || m_map.getBiome(positionFellow.x, positionFellow.y) != biome) {
        positionFellow = glm::vec3(random(min, max), random(min, max), zPos);
    }

    return positionFellow;
}

void BoidsManager::placeRabbitGroup(Biome biomeType)
{
    glm::vec3 positionMaster = computeBiomeLeaderPosition(biomeType, 0, MAP_SIZE, 0.0);
    glm::vec3 positionFellow;

    MovableBoidPtr leaderRabbit = addMovableBoid(RABBIT, positionMaster, positionMaster);
    leaderRabbit->setNewLeader(leaderRabbit);

    int nbRabbit = randInt(NB_RABBIT_MIN, NB_RABBIT_MAX);

    for (int i = 0; i < nbRabbit; ++i) {
        positionFellow = computeBiomeFellowPosition(biomeType, positionMaster, 0, MAP_SIZE, 0.0, 10.0f);
        MovableBoidPtr rabbitFellow = addMovableBoid(RABBIT, positionFellow, positionMaster);
        rabbitFellow->setNewLeader(leaderRabbit);
    }
}

void BoidsManager::placeWolfGroup(Biome biomeType)
{
    glm::vec3 positionMaster = computeBiomeLeaderPosition(biomeType, 0, MAP_SIZE, 2.0);
    glm::vec3 positionFellow;

    MovableBoidPtr leaderWolf = addMovableBoid(WOLF, positionMaster, positionMaster);
    leaderWolf->setNewLeader(leaderWolf);

    int nbWolf = randInt(NB_WOLF_MIN, NB_WOLF_MAX);

    for (int i = 0; i < nbWolf; ++i) {
        positionFellow = computeBiomeFellowPosition(biomeType, positionMaster, 0, MAP_SIZE, 2.0, 10.0f);
        MovableBoidPtr wolfFellow = addMovableBoid(WOLF, positionFellow, positionMaster);
        wolfFellow->setNewLeader(leaderWolf);
    }
}

void BoidsManager::placeForest(Biome biomeType)
{
    glm::vec3 position = computeBiomeLeaderPosition(biomeType, 0, MAP_SIZE, 2.0);
    addRootedBoid(TREE, position);

    glm::vec3 positionFellow;

    int nbTree = randInt(NB_TREE_MIN, NB_TREE_MAX);

    for (int i = 0; i < nbTree - 1; ++i) {
        positionFellow = computeBiomeFellowPosition(biomeType, position, 0, MAP_SIZE, 2.0, 25.0f);
        addRootedBoid(TREE, positionFellow);
    }
}

void BoidsManager::placeCarrotField(Biome biomeType)
{
    glm::vec3 position = computeBiomeLeaderPosition(biomeType, 0, MAP_SIZE, 2.0);
    addRootedBoid(CARROT, position);

    glm::vec3 positionFellow;

    int nbCarrot = randInt(NB_CARROT_MIN, NB_CARROT_MAX);

    for (int i = 0; i < nbCarrot - 1; ++i) {
        positionFellow = computeBiomeFellowPosition(biomeType, position, 0, MAP_SIZE, 2.0, 30.0f);
        addRootedBoid(CARROT, positionFellow);
    }
}

void BoidsManager::placeBoids( Biome biomeType, int nbRabbitGroup, int nbWolfGroup, 
	int nbForest,  int nbCarrotField) 
{
    for(int i = 0; i < nbRabbitGroup; i++) {
        placeRabbitGroup(biomeType);
    }

    for(int i = 0; i < nbWolfGroup; i++) {
        placeWolfGroup(biomeType);
    }

    for(int i = 0; i < nbForest; i++) {
        placeForest(biomeType);
    }

    for(int i = 0; i < nbCarrotField; i++) {
        placeCarrotField(biomeType);
    }
}
