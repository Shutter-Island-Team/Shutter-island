#include "../../include/boids2D/RootedBoid.hpp"

RootedBoid::RootedBoid(glm::vec3 location, BoidType t)
	: RootedBoid(location, t, 1)
{

}

RootedBoid::RootedBoid(glm::vec3 location, BoidType t, int amountFood)
	: RootedBoid(location, t, 2.0f, amountFood)
{

}

RootedBoid::RootedBoid(glm::vec3 location, BoidType t, float radius)
	: RootedBoid(location, t, radius, 1)
{

}

RootedBoid::RootedBoid(glm::vec3 location, BoidType t, float radius, int amountFood)
	: Boid(location, t, amountFood), m_radius(radius)
{
	
}

float RootedBoid::getRadius() const
{
	return m_radius;
}
