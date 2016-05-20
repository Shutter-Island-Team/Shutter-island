#include "../../include/boids2D/Boid.hpp"

Boid::Boid(glm::vec3 position, TypeBoid t) {
	m_location = position;
	m_typeBoid = t;
	m_movable = (m_typeBoid == RABBIT || m_typeBoid == WOLF);
}

glm::vec3 Boid::getLocation() {
	return m_location;
}

void Boid::setLocation(glm::vec3 newLocation) {
	m_location = newLocation;
}

bool Boid::isMovable() {
	return m_movable;
}

float Boid::getDistanceMin() {
	return m_distanceMin;
}

float Boid::getAngle() {
	return m_angle;
}

void Boid::setAngle(float angle) {
	m_angle = angle;
}

void Boid::setTarget(float x, float y) {
	m_target = glm::vec2(x, y);
}

glm::vec2 Boid::getTarget() {
	return m_target;
}

