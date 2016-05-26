#include "../../include/boids2D/RootedBoid.hpp"

RootedBoid::RootedBoid(glm::vec3 location, BoidType t)
	: Boid(location, t), m_radius(1.0f)
{

}

float RootedBoid::getRadius() const
{
	return m_radius;
}
