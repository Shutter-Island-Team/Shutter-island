#include "../../include/boids2D/Boid.hpp"

Boid::Boid(glm::vec3 position, TypeBoid t) {
	m_location = position;
	m_typeBoid = t;
}

glm::vec3 Boid::getLocation() {
	return m_location;
}

void Boid::setLocation(glm::vec3 newLocation) {
	m_location = newLocation;
}
