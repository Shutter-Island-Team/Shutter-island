#include <iostream>
#include <cmath>

#include "../../include/boids2D/MovableBoid.hpp"

#include "../../include/Utils.hpp"

MovableBoid::MovableBoid(glm::vec3 location, BoidType t, MovableParameters* parameters) 
	: MovableBoid(location, glm::vec3(0,0,0), t, parameters)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, BoidType t, MovableParameters* parameters) 
	: MovableBoid(location, velocity, 0.05f, t, parameters)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass, BoidType t, MovableParameters* parameters)
	: MovableBoid(location, velocity, mass, 3*M_PI/4, 0.3f, 2.0f, t, parameters)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, BoidType t, MovableParameters* parameters)
	: MovableBoid(location, velocity, mass, angleView, distViewSeparate, distViewCohesion, 3.5f, 2.0f, t, parameters)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, float maxSpeed, 
    float maxForce, BoidType t, MovableParameters* parameters)
	: Boid(location, t), m_velocity(velocity), 
	m_acceleration(glm::vec3(0,0,0)), m_mass(mass),
	m_angleView(angleView), m_distViewSeparate(distViewSeparate),
	m_distViewCohesion(distViewCohesion),
	m_maxSpeed(maxSpeed), m_maxForce(maxForce),
	m_parameters(parameters), m_stateType(WALK)
{
	switch(m_stateType) {
		case WALK:
			m_currentState = new WalkState();
			break;
		default:
			std::cerr << "State not recognize. Default state walk initialization" << std::endl;
			m_currentState = new WalkState();
			break;
	}
}

void MovableBoid::initializeParameters(MovableBoidPtr thisBoid)
{
	m_parameters->initializeParameters(thisBoid);
}

glm::vec3 MovableBoid::getVelocity(){
	return m_velocity;
}

glm::vec3 MovableBoid::getAcceleration() {
	return m_acceleration;
}

float MovableBoid::getMass() {
	return m_mass;
}

MovableParameters & MovableBoid::getParameters() {
	return *m_parameters;
}

void MovableBoid::resetAcceleration() {
	m_acceleration = glm::vec3(0, 0, 0);
}

void MovableBoid::computeNextStep(float dt) {
    m_velocity = limitVec3(m_velocity + (dt / m_mass) * limitVec3(m_acceleration, m_maxForce), m_maxSpeed);
	setAngle(atan2(m_velocity.y, m_velocity.x));
    m_location += dt * m_velocity;
}

bool MovableBoid::canSee(Boid b, float distView) {
	return (glm::distance(m_location, b.getLocation()) < distView) && (angleVision(b));
}

bool MovableBoid::angleVision (Boid b) {
	glm::vec3 diffPos = b.getLocation() - m_location;
	float comparativeValue = acos(glm::dot(glm::normalize(m_velocity), glm::normalize(diffPos)));

	if (m_angleView <= M_PI) {
		return (0 <= comparativeValue) && (comparativeValue <= m_angleView/2);
	} else {
        diffPos = - diffPos;
        comparativeValue = - comparativeValue;
        return !((0 <= comparativeValue) && (comparativeValue <= M_PI - m_angleView/2));
    }
}

void MovableBoid::walk(std::vector<MovableBoidPtr> mvB) {
	if (m_parameters->isLowStamina()) {
		delete m_currentState;
		m_currentState = new StayState();
		m_stateType = STAY;
	} else {

	}
}

void MovableBoid::stay(std::vector<MovableBoidPtr> mvB) {
	if (m_parameters->isHighStamina()) {
		delete m_currentState;
		m_currentState = new WalkState();
		m_stateType = WALK;
	} else {

	}
}

void MovableBoid::computeAcceleration (std::vector<MovableBoidPtr> mvB) {
	std::cerr << "Value of stamina : " << m_parameters->getStamina() << std::endl;
	switch (m_stateType) {
		case WALK:
			walk(mvB);
			break;
		case STAY:
			stay(mvB);
			break;
		default:
			std::cerr << "Unknown state" << std::endl;
			break;
	}
	setAcceleration(m_currentState->computeAcceleration(*this, mvB));
}

glm::vec3 MovableBoid::ruleStayWithinWalls() {
	glm::vec3 steer(0, 0, 0);
	float distToWall = 20.0f;
    if (m_location.x < -distToWall) {
      glm::vec3 desired(m_maxSpeed, m_velocity.y, 0);
      steer = desired - m_velocity;
    } else if (m_location.x >  distToWall) {
      glm::vec3 desired(-m_maxSpeed, m_velocity.y, 0);
      steer = desired - m_velocity ;
    }
    
    if (m_location.y < -distToWall) {
      glm::vec3 desired(m_velocity.x, m_maxSpeed, 0);
      steer += desired - m_velocity;
    } else if (m_location.y >  distToWall) {
      glm::vec3 desired(m_velocity.x, -m_maxSpeed, 0);
      steer += desired - m_velocity;
    }

    steer = limitVec3(steer, m_maxForce);

    return steer;
}

glm::vec3 MovableBoid::separate(std::vector<MovableBoidPtr> mvB) {
	glm::vec3 sum;
	int count = 0;
	for(MovableBoidPtr m : mvB) {
		float d = glm::distance(getLocation(), m->getLocation());
		if ((d > 0) && canSee(*m, m_distViewSeparate)) {
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

glm::vec3 MovableBoid::align (std::vector<MovableBoidPtr> mvB) {
	glm::vec3 sum(0,0,0);
	int count = 0;
	for (MovableBoidPtr other : mvB) {
		float d = glm::distance(getLocation(), other->getLocation());
		if ((d > 0) && (d < m_distViewCohesion) && canSee(*other, m_angleView)) {
			sum += other->m_velocity;
			// For an average, we need to keep track of
			// how many boids are within the distance.
			count++;
		}
	}
	if (count > 0) {
		sum = sum / (float) count;
		// sum.normalize();
		// sum.mult(maxspeed);
		return glm::normalize(limitVec3(sum - m_velocity, m_maxForce));
	}
	return glm::vec3(0,0,0);
}

glm::vec3 MovableBoid::cohesion (std::vector<MovableBoidPtr> mvB) {
    glm::vec3 sum(0,0,0);
    int count = 0;
    for (MovableBoidPtr other : mvB) {
		float d = glm::distance(getLocation(), other->getLocation());
		if ((d > 0) && (d < m_distViewCohesion) && canSee(*other, m_angleView)) {
			// Adding up all the others’ locations
			sum += other->getLocation();
			count++;
		}
    }
    if (count > 0) {
		sum /= (float) count;
		// Here we make use of the seek() function we
		// wrote in Example 6.8.  The target
		// we seek is the average location of
		// our neighbors.
		// return arrive(sum);
        return glm::vec3(0,0,0);
    }

    return glm::vec3(0,0,0);
}

bool operator==(const MovableBoid& b1, const MovableBoid& b2) {
	return b1.getLocation() == b2.getLocation();
}

bool operator!=(const MovableBoid& b1, const MovableBoid& b2){
	return !(b1 == b2);
}

void MovableBoid::setAcceleration(glm::vec3 acceleration) {
	m_acceleration = acceleration;
}
