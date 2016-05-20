#include "../../include/boids2D/Wolf.hpp"
#include "../../include/boids2D/BoidType.hpp"

Wolf::Wolf(glm::vec3 location) 
	: Wolf(location, glm::vec3(0,0,0))
{

}

Wolf::Wolf(glm::vec3 location, glm::vec3 velocity) 
	: Wolf(location, velocity, 0.05f)
{

}

Wolf::Wolf(glm::vec3 location, glm::vec3 velocity, float mass)
	: Wolf(location, velocity, mass, 3*M_PI/4, 0.3f, 2.0f)
{

}

Wolf::Wolf(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion)
	: Wolf(location, velocity, mass, angleView, distViewSeparate, distViewCohesion, 3.5f, 2.0f)
{

}

Wolf::Wolf(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, float maxSpeed, 
    float maxForce)
	: MovableBoid(location, velocity, mass, angleView, m_distViewSeparate, m_distViewCohesion, maxSpeed, maxForce, WOLF)
{

}
