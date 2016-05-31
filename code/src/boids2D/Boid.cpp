#include "../../include/boids2D/Boid.hpp"

Boid::Boid(glm::vec3 location, BoidType t) 
	: Boid(location, t, 1)
{

}

Boid::Boid(glm::vec3 location, BoidType t, int amountFood)
	: m_location(location), m_boidType(t), m_angle(0.0f), m_scale(1.0f), m_display(true), m_amountFood(1)
{
	
}

glm::vec3 Boid::getLocation() const
{
	return m_location;
}

void Boid::setLocation(const glm::vec3 & location)
{
	m_location = location;
}

float Boid::getAngle() const
{
	return m_angle;
}

void Boid::setAngle(const float & angle)
{
	m_angle = angle;
}

BoidType Boid::getBoidType() const
{
	return m_boidType;
}

float Boid::getScale() const
{
	return m_scale;
}

void Boid::setScale(const float & scale)
{
	m_scale = scale;
}

void Boid::disapear()
{
	m_display = false;
}

bool Boid::toDisplay() const
{
	return m_display;
}

bool Boid::isFoodRemaining() const
{
	return m_amountFood > 0.0f;
}

void Boid::decreaseFoodRemaining()
{
	///< Check this coefficient
	m_amountFood--;
}
