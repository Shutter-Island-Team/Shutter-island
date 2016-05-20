#include <iostream>
#include <cmath>

#include "../../include/boids2D/MovableBoid.hpp"

#include "../../include/Utils.hpp"

// TODO : steering + solver. The force send should be only normalize in the end

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

void MovableBoid::computeNextStep(float dt) {
    m_velocity += (dt / m_mass) * limitVec3(m_acceleration, m_maxForce);
    m_velocity = limitVec3(m_velocity, m_maxSpeed);
    setAngle(atan2(m_velocity.y, m_velocity.x));
    setLocation( getLocation() + dt * m_velocity );

    // Borderless
    glm::vec3 locationInBox = getLocation() + glm::vec3(20, 20, 20);
    locationInBox.x = fmod(locationInBox.x + 20, 20);
    locationInBox.y = fmod(locationInBox.y + 20, 20);
    locationInBox.z = fmod(locationInBox.z + 20, 20);

    setLocation( locationInBox );
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

	if (getTarget().x == 0) {
		m_acceleration = 20.0f * wander();
	} else {
		m_acceleration = arrive(glm::vec3(getTarget().x, getTarget().y, 2));
	}
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

glm::vec3 MovableBoid::wander() {
	float randomVal = random(0.0f, 2*M_PI);
	glm::vec3 randomVec3(cos(randomVal), sin(randomVal), 0);
    glm::vec3 desiredTarget = getLocation() + distToCircle*m_velocity + rCircleWander*randomVec3;
    return arrive(desiredTarget);
}

glm::vec3 MovableBoid::separate(std::vector<MovableBoid> mvB, float desiredSeparation) {
	glm::vec3 sum;
	int count = 0;
	for(MovableBoid m : mvB) {
		float d = glm::distance(getLocation(), m.getLocation());
		if ((d > 0) && (d < desiredSeparation)) {
			glm::vec3 diff = getLocation() - m.getLocation();
			diff = glm::normalize(diff) / d;
			sum += diff;
			count++;
		}
		if (count > 0) {
			sum /= count;
			sum = glm::normalize(sum) * m_maxSpeed;
			glm::vec3 steer = sum - m_velocity;
			return limitVec3(steer, m_maxForce);
		}
		return glm::vec3(0, 0, 0);
	}
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
