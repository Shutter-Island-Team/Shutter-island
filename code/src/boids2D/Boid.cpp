#include "../../include/boids2D/Boid.hpp"

Boid::Boid(glm::vec3 location, BoidType t) 
	: m_location(location), m_boidType(t) {
	m_movable = (m_boidType == RABBIT || m_boidType == WOLF); // TODO do it with a function
}

glm::vec3 Boid::getLocation() {
	return m_location;
}

void Boid::setLocation(glm::vec3 location) {
	m_location = location;
}

bool Boid::isMovable() {
	return m_movable;
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

float Boid::getMinDistance() {
	return m_minDistance;
}

void Boid::setMinDistance(float minDistance) {
	m_minDistance = minDistance;
}

BoidType Boid::getBoidType() {
	return m_boidType;
}
