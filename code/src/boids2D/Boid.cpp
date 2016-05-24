#include "../../include/boids2D/Boid.hpp"

Boid::Boid(glm::vec3 location, BoidType t) 
	: m_location(location), m_boidType(t)
{

}

glm::vec3 Boid::getLocation() const {
	return m_location;
}

void Boid::setLocation(const glm::vec3 & location) {
	m_location = location;
}

float Boid::getAngle() const {
	return m_angle;
}

void Boid::setAngle(const float & angle) {
	m_angle = angle;
}

float Boid::getMinDistance() const {
	return m_minDistance;
}

void Boid::setMinDistance(const float & minDistance) {
	m_minDistance = minDistance;
}

BoidType Boid::getBoidType() const {
	return m_boidType;
}
