#include "../../include/boids2D/Rabbit.hpp"
#include "../../include/boids2D/BoidType.hpp"

Rabbit::Rabbit(glm::vec3 location, MovableParameters* parameters) 
	: Rabbit(location, glm::vec3(0,0,0), parameters)
{

}

Rabbit::Rabbit(glm::vec3 location, glm::vec3 velocity, MovableParameters* parameters) 
	: Rabbit(location, velocity, 0.05f, parameters)
{

}

Rabbit::Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
    MovableParameters* parameters)
	: MovableBoid(location, velocity, mass, RABBIT, parameters, 1)
{

}
