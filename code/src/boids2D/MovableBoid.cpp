#include <cmath>

#include "../../include/boids2D/MovableBoid.hpp"

#include "../../include/Utils.hpp"

// TODO : steering + solver. The force send should be only normalize in the end

MovableBoid::MovableBoid(glm::vec3 position, TypeBoid t) 
: Boid(position, t) {
	// TODO
	// HERE
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

void MovableBoid::computeNextStep(float dt) {
    m_velocity += (dt / m_mass) * limitVec3(m_acceleration, m_maxSpeed);
    m_velocity = limitVec3(m_velocity, m_maxSpeed);
    setLocation( getLocation() + dt * m_velocity );
}

glm::vec3 MovableBoid::getVelocity(){
	return m_velocity;
}

void MovableBoid::setVelocity(glm::vec3 velocity) {
	m_velocity = velocity;
}

float MovableBoid::getMass() {
	return m_mass;
}

glm::vec3 MovableBoid::computeAcceleration () {
	// Reset acceleration
	m_acceleration = glm::vec3(0, 0, 0);
	// applyForce(arrive(glm::vec3(0, 0, 2)));
	glm::vec3 wanderVec = wander();
	glm::vec3 stayWithinWalls = 4.0f * ruleStayWithinWalls();
	applyForce(wanderVec + stayWithinWalls);
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

glm::vec3 MovableBoid::ruleStayWithinWalls() {
	glm::vec3 steer(0, 0, 0);
	float distToWall = 20.0f;
    if (getLocation().x < -distToWall) {
      glm::vec3 desired(m_maxSpeed, m_velocity.y, 0);
      steer = desired - m_velocity;
    } else if (getLocation().x >  distToWall) {
      glm::vec3 desired(m_maxSpeed, -m_velocity.y, 0);
      steer = m_velocity - desired; // TODO : This is horrible and not understandible at all
    }
    
    if (getLocation().y < -distToWall) {
      glm::vec3 desired(m_maxSpeed, m_velocity.x, 0);
      steer = desired + steer - m_velocity;
    } else if (getLocation().y >  distToWall) {
      glm::vec3 desired(m_maxSpeed, -m_velocity.x, 0);
      steer = m_velocity + steer - desired; // TODO : This is horrible and not understandible at all
    }

    steer = limitVec3(steer, m_maxForce);

    return steer;
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

glm::vec3 MovableBoid::wander() {
	float randomVal = random(0.0f, 2*M_PI);
	glm::vec3 randomVec3(cos(randomVal), sin(randomVal), 0);
    glm::vec3 desiredTarget = getLocation() + distToCircle*m_velocity + rCircleWander*randomVec3;
    return arrive(desiredTarget);
}


glm::vec3 MovableBoid::arrive(glm::vec3 target) {
	glm::vec3 desired = target - getLocation();

	float d = glm::length(desired);
	glm::normalize(desired);
	if (d < distStartSlowingDown) {
	  // Set the magnitude according to how close we are.
	  float m = d*m_maxSpeed/distStartSlowingDown;
	  desired *= m;
	} else {
	  // Otherwise, proceed at maximum speed.
	  desired *= m_maxSpeed;
	}

	// The usual steering = desired - velocity
	glm::vec3 steer = desired - m_velocity;
	if (glm::length(steer) > m_maxForce) {
		steer = glm::normalize(steer)*m_maxForce;
	}

	return steer;
}

glm::vec3 MovableBoid::getAcceleration() {
	return m_acceleration;
}
