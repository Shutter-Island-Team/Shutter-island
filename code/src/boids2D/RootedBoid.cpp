#include "../../include/boids2D/RootedBoid.hpp"

RootedBoid::RootedBoid(glm::vec3 location, BoidType t)
	: RootedBoid(location, t, 1)
{

}

RootedBoid::RootedBoid(glm::vec3 location, BoidType t, int amountFood)
	: Boid(location, t, amountFood), m_radius(2.0f)
{

}

float RootedBoid::getRadius() const
{
	return m_radius;
}
