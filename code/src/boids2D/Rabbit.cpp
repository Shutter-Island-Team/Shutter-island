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

Rabbit::Rabbit(glm::vec3 location, glm::vec3 velocity, float mass, MovableParameters* parameters)
	: Rabbit(location, velocity, mass, 3*M_PI/4, 0.3f, 2.0f, parameters)
{

}

Rabbit::Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, MovableParameters* parameters)
	: Rabbit(location, velocity, mass, angleView, distViewSeparate, distViewCohesion, 3.5f, 2.0f, parameters)
{

}

Rabbit::Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, float maxSpeed, 
    float maxForce, MovableParameters* parameters)
	: MovableBoid(location, velocity, mass, angleView, m_distViewSeparate, m_distViewCohesion, maxSpeed, maxForce, RABBIT, parameters)
{

}
