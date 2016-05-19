#include "../../include/boids2D/MovableBoid.hpp"

MovableBoid::MovableBoid(glm::vec3 position, TypeBoid t) 
: Boid(position, t) {
	// TODO
	// HERE
}

void MovableBoid::computeNextStep() {
	// TODO : to implement
}

glm::vec3 MovableBoid::getVelocity(){
	return m_velocity;
}

void MovableBoid::move (std::vector<MovableBoid>& movableBoids){
	glm::vec3 f(0,0,0);

	f = ruleFlyToCenter(movableBoids) 
		+ ruleKeepDistance(movableBoids)
		+ ruleMatchSpeed(movableBoids);

	applyForce(f);
	update();
}

bool MovableBoid::isNeighbor(Boid b) {
	// TODO
	return true;
}

bool MovableBoid::isNear(Boid b) {
	// TODO
	return true;
}

glm::vec3 MovableBoid::ruleFlyToCenter(std::vector<MovableBoid>& movableBoids){
	glm::vec3 sum(0,0,0);
	int count = 0;

	for(size_t i=0; i<movableBoids.size(); ++i) {
	 	MovableBoid b = movableBoids[i];
	 	if(isNeighbor(b)) {
	 		sum += b.getLocation();
	 		count++;
	 	}
	}

	if(count > 0){ 
		sum /= count;
		glm::normalize(sum);
		sum *= m_normalSpeed;
	}

	return sum;

}

glm::vec3 MovableBoid::ruleKeepDistance(std::vector<MovableBoid>& movableBoids){
	glm::vec3 sum(0,0,0);

	for(size_t i=0; i<movableBoids.size(); ++i) {
	 	MovableBoid b = movableBoids[i];
	 	if(isNear(b)) {
	 		if(glm::distance(getLocation(), b.getLocation()) < getDistanceMin())
	 		sum = sum + (getLocation() - b.getLocation());
	 	}
	}

	return sum;
}

glm::vec3 MovableBoid::ruleMatchSpeed(std::vector<MovableBoid>& movableBoids){
	glm::vec3 sum(0,0,0);
	int count = 0;

	for(size_t i=0; i<movableBoids.size(); ++i) {
	 	MovableBoid b = movableBoids[i];
	 	if(isNeighbor(b)) {
	 		sum += b.getVelocity();
	 		count++;
	 	}
	}

	if(count > 0){ 
		sum /= count;
		sum -= m_velocity;
	}

	return sum;

}


void MovableBoid::applyForce(glm::vec3 f){
	// TODO function limit
	glm::vec3 tmp(0,0,0);
	tmp.x = std::min(f.x, m_maxForce);
	tmp.y = std::min(f.y, m_maxForce);
	tmp.z = std::min(f.z, m_maxForce);

	m_acceleration = tmp / m_mass;
}

void MovableBoid::update(){
	m_velocity += m_acceleration;
	// TODO function limit
	m_velocity.x = std::min(m_velocity.x, m_maxSpeed);
	m_velocity.y = std::min(m_velocity.y, m_maxSpeed);
	m_velocity.z = std::min(m_velocity.z, m_maxSpeed);

	setLocation(getLocation() + m_velocity);

	m_acceleration.x = 0;
	m_acceleration.y = 0;
	m_acceleration.z = 0;
}
