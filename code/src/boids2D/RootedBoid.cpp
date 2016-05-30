#include "../../include/boids2D/RootedBoid.hpp"

RootedBoid::RootedBoid(glm::vec3 location, BoidType t)
	: Boid(location, t), m_radius(2.0f)
{

}

float RootedBoid::getRadius() const
{
	return m_radius;
}

void RootedBoid::disapear()
{
	///< @todo : remove from vector
	m_location = glm::vec3(1000,1000,1000);
}