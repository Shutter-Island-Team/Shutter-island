#include "../../include/boids2D/BoidsManager.hpp"

BoidsManager::BoidsManager(void) 
{
	
}

MovableBoidPtr BoidsManager::addMovableBoid(BoidType boidType, glm::vec3 location, glm::vec3 velocity) 
{
	MovableBoidPtr movableBoid;
	MovableParameters *parameters = new MovableParameters(boidType);

	switch(boidType)
	{
		case WOLF:
			movableBoid = std::make_shared<Wolf>(location, velocity, parameters);
			break;
		case RABBIT:
			movableBoid = std::make_shared<Rabbit>(location, velocity, parameters);
			break;
		default:
			throw std::invalid_argument("valid boidType required");
			break;
	}
    
    movableBoid->associateBoid(movableBoid);
    m_movableBoids.push_back(movableBoid);
    
    return movableBoid;
}

RootedBoidPtr BoidsManager::addRootedBoid(BoidType boidType, glm::vec3 location)
{
	RootedBoidPtr rootedBoid;
	switch(boidType)
	{
		case CARROT:
			rootedBoid = std::make_shared<Carrot>(location);
			break;
		case TREE:
			rootedBoid = std::make_shared<Tree>(location);
			break;
		default:
			throw std::invalid_argument("valid boidType required");
			break;
	}

	m_rootedBoids.push_back(rootedBoid);
	
	return rootedBoid;
}


std::vector<MovableBoidPtr>& BoidsManager::getMovableBoids()
{
	return m_movableBoids;
}

std::vector<RootedBoidPtr>& BoidsManager::getRootedBoids()
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

