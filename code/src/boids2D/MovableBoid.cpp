#include <cmath>

#include "../../include/boids2D/MovableBoid.hpp"

MovableBoid::MovableBoid(glm::vec3 position, TypeBoid t) 
: Boid(position, t) {

}

bool MovableBoid::canSee(Boid b) {
	return (glm::distance(getLocation(), b.getLocation()) < m_distView) && (angleVision(b));
}

bool MovableBoid::angleVision (Boid b) {
	glm::vec3 diffPos = b.getLocation() - getLocation();
	float comparativeValue = acos(glm::dot(glm::normalize(getVelocity()), glm::normalize(diffPos)));

	if (m_angleView <= M_PI) {
		return (0 <= comparativeValue) && (comparativeValue <= m_angleView/2);
	} else {
        diffPos = - diffPos;
        comparativeValue = -comparativeValue;
        return !((0 <= comparativeValue) && (comparativeValue <= M_PI - m_angleView/2));
    }
}

void MovableBoid::computeNextStep() {
	// TODO : to implement
}