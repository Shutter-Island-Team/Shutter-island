#include "../../include/boids2D/Rabbit.hpp"
#include "../../include/boids2D/BoidType.hpp"

Rabbit::Rabbit(glm::vec3 location) 
	: Rabbit(location, glm::vec3(0,0,0))
{

}

Rabbit::Rabbit(glm::vec3 location, glm::vec3 velocity) 
	: Rabbit(location, velocity, 0.05f)
{

}

Rabbit::Rabbit(glm::vec3 location, glm::vec3 velocity, float mass)
	: Rabbit(location, velocity, mass, 3*M_PI/4, 0.3f, 2.0f)
{

}

Rabbit::Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion)
	: Rabbit(location, velocity, mass, angleView, distViewSeparate, distViewCohesion, 3.5f, 2.0f)
{

}

Rabbit::Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, float maxSpeed, 
    float maxForce)
	: MovableBoid(location, velocity, mass, angleView, m_distViewSeparate, m_distViewCohesion, maxSpeed, maxForce, RABBIT)
{

}
