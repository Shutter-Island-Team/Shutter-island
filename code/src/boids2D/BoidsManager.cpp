#include "../../include/boids2D/BoidsManager.hpp"

BoidsManager::BoidsManager(void) 
{
	
}

// Warning : the shader has to be a texture shader
void BoidsManager::addMovableBoid(BoidType boidType, glm::vec3 location, glm::vec3 velocity) 
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
    /*
    BoidRenderablePtr boidRenderable = std::make_shared<BoidRenderable>(shader, movableBoid);
    boidRenderable->setMaterial(Material::Pearl());
    HierarchicalRenderable::addChild( systemRenderable, boidRenderable );
    */
}

// Warning : the shader has to be a texture shader
void BoidsManager::addRootedBoid(BoidType boidType, glm::vec3 location)
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
	/*
    BoidRenderablePtr boidRenderable = std::make_shared<BoidRenderable>(shader, rootedBoid);
    boidRenderable->setMaterial(Material::Pearl());
    HierarchicalRenderable::addChild( systemRenderable, boidRenderable );
	*/
}

// Warning : the shader has to be a texture shader
void BoidsManager::addBoid(BoidType boidType, glm::vec3 location, glm::vec3 velocity)
{
	switch(boidType)
	{
		case WOLF:
		case RABBIT:
			addMovableBoid(boidType, location, velocity);
			break;
		case CARROT:
		case TREE:
			addRootedBoid(boidType, location);
			break;
		default:
			throw std::invalid_argument("valid boidType required");
			break;
	}
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

