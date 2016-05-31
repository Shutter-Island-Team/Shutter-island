#include "../../include/boids2D/Wolf.hpp"
#include "../../include/boids2D/BoidType.hpp"

Wolf::Wolf(glm::vec3 location, MovableParameters* parameters) 
	: Wolf(location, glm::vec3(0,0,0), parameters)
{

}

Wolf::Wolf(glm::vec3 location, glm::vec3 velocity, MovableParameters* parameters) 
	: Wolf(location, velocity, 0.05f, parameters)
{

}

Wolf::Wolf(glm::vec3 location, glm::vec3 velocity, float mass,
    MovableParameters* parameters)
	: MovableBoid(location, velocity, mass, WOLF, parameters, 300.0f)
{

}
