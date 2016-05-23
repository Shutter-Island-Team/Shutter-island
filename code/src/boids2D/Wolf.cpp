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

Wolf::Wolf(glm::vec3 location, glm::vec3 velocity, float mass, MovableParameters* parameters)
	: Wolf(location, velocity, mass, 3*M_PI/4, 0.3f, 2.0f, parameters)
{

}

Wolf::Wolf(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, MovableParameters* parameters)
	: Wolf(location, velocity, mass, angleView, distViewSeparate, distViewCohesion, 3.5f, 2.0f, parameters)
{

}

Wolf::Wolf(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, float maxSpeed, 
    float maxForce, MovableParameters* parameters)
	: MovableBoid(location, velocity, mass, angleView, m_distViewSeparate, m_distViewCohesion, maxSpeed, maxForce, WOLF, parameters)
{

}
