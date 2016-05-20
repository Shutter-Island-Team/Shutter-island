#include <iostream>
#include <cmath>

#include "../../include/boids2D/MovableBoid.hpp"

#include "../../include/Utils.hpp"

// TODO : steering + solver. The force send should be only normalize in the end

MovableBoid::MovableBoid(glm::vec3 location, BoidType t) 
	: Boid(location, t), m_velocity(glm::vec3(0,0,0)), 
	m_acceleration(glm::vec3(0,0,0)), m_mass(DEFAULT_MASS),
	m_angleView(DEFAULT_ANGLE_VIEW), m_distView(DEFAULT_DISTANCE_VIEW), 
	m_maxSpeed(DEFAULT_MAX_SPEED), m_maxForce(DEFAULT_MAX_FORCE)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, BoidType t) 
	: Boid(location, t), m_velocity(velocity), 
	m_acceleration(glm::vec3(0,0,0)), m_mass(DEFAULT_MASS),
	m_angleView(DEFAULT_ANGLE_VIEW), m_distView(DEFAULT_DISTANCE_VIEW), 
	m_maxSpeed(DEFAULT_MAX_SPEED), m_maxForce(DEFAULT_MAX_FORCE)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass, BoidType t)
	: Boid(location, t), m_velocity(velocity), 
	m_acceleration(glm::vec3(0,0,0)), m_mass(mass),
	m_angleView(DEFAULT_ANGLE_VIEW), m_distView(DEFAULT_DISTANCE_VIEW), 
	m_maxSpeed(DEFAULT_MAX_SPEED), m_maxForce(DEFAULT_MAX_FORCE)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distView, BoidType t)
	: Boid(location, t), m_velocity(velocity), 
	m_acceleration(glm::vec3(0,0,0)), m_mass(mass),
	m_angleView(angleView), m_distView(distView), 
	m_maxSpeed(DEFAULT_MAX_SPEED), m_maxForce(DEFAULT_MAX_FORCE)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distView, float maxSpeed, 
    float maxForce, BoidType t)
	: Boid(location, t), m_velocity(velocity), 
	m_acceleration(glm::vec3(0,0,0)), m_mass(mass),
	m_angleView(angleView), m_distView(distView), 
	m_maxSpeed(maxSpeed), m_maxForce(maxForce)
{

}

bool MovableBoid::canSee(Boid b) {
	return (glm::distance(m_location, b.getLocation()) < m_distView) && (angleVision(b));
}

bool MovableBoid::angleVision (Boid b) {
	glm::vec3 diffPos = b.getLocation() - m_location;
	float comparativeValue = acos(glm::dot(glm::normalize(m_velocity), glm::normalize(diffPos)));

	if (m_angleView <= M_PI) {
		return (0 <= comparativeValue) && (comparativeValue <= m_angleView/2);
	} else {
        diffPos = - diffPos;
        comparativeValue = -comparativeValue;
        return !((0 <= comparativeValue) && (comparativeValue <= M_PI - m_angleView/2));
    }
}

void MovableBoid::computeNextStep(float dt) {
    m_velocity = limitVec3(m_velocity + (dt / m_mass) * limitVec3(m_acceleration, m_maxForce), m_maxSpeed);
	setAngle(atan2(m_velocity.y, m_velocity.x));
    m_location += dt * m_velocity;
}

glm::vec3 MovableBoid::getVelocity(){
	return m_velocity;
}

float MovableBoid::getMass() {
	return m_mass;
}

glm::vec3 MovableBoid::computeAcceleration (std::vector<MovableBoidPtr> mvB) {
	// Reset acceleration
	m_acceleration = glm::vec3(0, 0, 0);

	if (getTarget().x == 0) {
		glm::vec3 wanderVec = 1.0f * wander();
		glm::vec3 separateVec = 5.0f * separate(mvB, 2.0f);
		glm::vec3 stayWithinWallsVec = 14.0f * ruleStayWithinWalls();

		m_acceleration = wanderVec + separateVec + stayWithinWallsVec;
	} else {
		glm::vec3 seek = 1.0f * arrive(glm::vec3(getTarget().x, getTarget().y, 2));
		glm::vec3 separateVec = 5.0f * separate(mvB, 2.0f);

		m_acceleration = seek + separateVec;
	}
}

glm::vec3 MovableBoid::ruleStayWithinWalls() {
	glm::vec3 steer(0, 0, 0);
	float distToWall = 20.0f;
    if (m_location.x < -distToWall) {
      glm::vec3 desired(m_maxSpeed, m_velocity.y, 0);
      steer = desired - m_velocity;
    } else if (m_location.x >  distToWall) {
      glm::vec3 desired(m_maxSpeed, -m_velocity.y, 0);
      steer = m_velocity - desired; // TODO : This is horrible and not understandible at all
    }
    
    if (m_location.y < -distToWall) {
      glm::vec3 desired(m_maxSpeed, m_velocity.x, 0);
      steer = desired + steer - m_velocity;
    } else if (m_location.y >  distToWall) {
      glm::vec3 desired(m_maxSpeed, -m_velocity.x, 0);
      steer = m_velocity + steer - desired; // TODO : This is horrible and not understandible at all
    }

    steer = limitVec3(steer, m_maxForce);

    return steer;
}

glm::vec3 MovableBoid::wander() {
	float randomVal = random(0.0f, 2*M_PI);
	glm::vec3 randomVec3(cos(randomVal), sin(randomVal), 0);
    glm::vec3 desiredTarget = m_location + distToCircle*m_velocity + rCircleWander*randomVec3;
    return arrive(desiredTarget);
}

glm::vec3 MovableBoid::separate(std::vector<MovableBoidPtr> mvB, float desiredSeparation) {
	glm::vec3 sum;
	int count = 0;
	for(MovableBoidPtr m : mvB) {
		float d = glm::distance(getLocation(), m->getLocation());
		if ((d > 0) && (d < desiredSeparation)) {
			glm::vec3 diff = getLocation() - m->getLocation();
			diff = glm::normalize(diff) / d;
			sum += diff;
			count++;
		}
	}
	if (count > 0) {
		sum /= count;
		sum = glm::normalize(sum) * m_maxSpeed;
		glm::vec3 steer = sum - m_velocity;
		return limitVec3(steer, m_maxForce);
	}
	return glm::vec3(0, 0, 0);
}

glm::vec3 MovableBoid::arrive(glm::vec3 target) {
	glm::vec3 desired = target - m_location;

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
