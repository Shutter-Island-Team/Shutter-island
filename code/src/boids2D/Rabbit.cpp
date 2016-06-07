#include "../../include/boids2D/Rabbit.hpp"
#include "../../include/boids2D/BoidType.hpp"

Rabbit::Rabbit(glm::vec3 location, glm::vec3 landmarkPosition, MovableParametersPtr parameters) 
	: Rabbit(location, landmarkPosition, glm::vec3(0,0,0), parameters)
{

}

Rabbit::Rabbit(glm::vec3 location, glm::vec3 landmarkPosition, glm::vec3 velocity, MovableParametersPtr parameters) 
	: Rabbit(location, landmarkPosition, velocity, 0.05f, parameters)
{

}

Rabbit::Rabbit(glm::vec3 location, glm::vec3 landmarkPosition, glm::vec3 velocity, float mass,
    MovableParametersPtr parameters)
	: MovableBoid(location, landmarkPosition, velocity, mass, RABBIT, parameters, 1)
{

}
