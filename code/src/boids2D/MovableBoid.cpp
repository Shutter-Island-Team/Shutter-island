#include <iostream>
#include <cmath>
#include "../../include/Utils.hpp"
#include "../../include/boids2D/MovableBoid.hpp"

MovableBoid::~MovableBoid()
{
	delete m_currentState;
	delete m_parameters;
}

MovableBoid::MovableBoid(glm::vec3 location, BoidType t, MovableParameters* parameters) 
	: MovableBoid(location, glm::vec3(0,0,0), t, parameters)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, BoidType t, MovableParameters* parameters) 
	: MovableBoid(location, velocity, 0.05f, t, parameters)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    BoidType t, MovableParameters* parameters)
	: Boid(location, t), m_velocity(velocity), 
	m_acceleration(glm::vec3(0,0,0)), m_mass(mass),
	m_parameters(parameters), m_stateType(WALK_STATE)
{
	switch(m_stateType) {
		case WALK_STATE:
			m_currentState = new WalkState();
			break;
		default:
			std::cerr << "State not recognize. Default state walk initialization" << std::endl;
			m_currentState = new WalkState();
			break;
	}
}

void MovableBoid::associateBoid(MovableBoidPtr boidPtr)
{
	m_parameters->associateBoid(boidPtr);
}

glm::vec3 MovableBoid::getVelocity() const
{
	return m_velocity;
}

float MovableBoid::getMass() const
{
	return m_mass;
}

MovableParameters & MovableBoid::getParameters() const
{
	return *m_parameters;
}

void MovableBoid::resetAcceleration()
{
	m_acceleration = glm::vec3(0, 0, 0);
}

void MovableBoid::computeAcceleration (const std::vector<MovableBoidPtr> & mvB)
{
	switch (m_stateType) {
		case WALK_STATE:
			walkStateHandler();
			break;
		case STAY_STATE:
			stayStateHandler();
			break;
		case FIND_FOOD_STATE:
			findFoodStateHandler();
			break;
		default:
			std::cerr << "Unknown state" << std::endl;
			break;
	}
	m_acceleration = m_currentState->computeAcceleration(*this, mvB);
}

void MovableBoid::computeNextStep(const float & dt)
{
    m_velocity = limitVec3(m_velocity + (dt / m_mass) * limitVec3(m_acceleration, getParameters().getMaxForce()), getParameters().getMaxSpeed());
	setAngle(atan2(m_velocity.y, m_velocity.x));
    m_location += dt * m_velocity;
}

bool MovableBoid::canSee(const Boid & other, const float & distView) const
{
	return (distVision(other, distView)) && (angleVision(other));
}

bool MovableBoid::distVision (const Boid & other, const float & distView) const
{
	return (glm::distance(m_location, other.getLocation()) < distView);
}

bool MovableBoid::sameSpecies(const Boid & other) const
{
	return other.getBoidType() == getBoidType();
}

bool MovableBoid::angleVision (const Boid & other) const
{
	glm::vec3 diffPos = other.getLocation() - m_location;
	float comparativeValue = acos(glm::dot(glm::normalize(m_velocity), glm::normalize(diffPos)));

	if (getParameters().getAngleView() <= M_PI) {
		return (0 <= comparativeValue) && (comparativeValue <= getParameters().getAngleView()/2);
	} else {
        diffPos = - diffPos;
        comparativeValue = - comparativeValue;
        return !((0 <= comparativeValue) && (comparativeValue <= M_PI - getParameters().getAngleView()/2));
    }
}

void MovableBoid::walkStateHandler() {
	/*
	if (m_parameters->isLowStamina()) {
		delete m_currentState;
		m_currentState = new StayState();
		m_stateType = STAY_STATE;
	} 
	*/
	if (m_parameters->isLowHunger()) {
		delete m_currentState;
		m_currentState = new FindFoodState();
		m_stateType = FIND_FOOD_STATE;
	}
}

void MovableBoid::stayStateHandler() {
	if (m_parameters->isHighStamina()) {
		delete m_currentState;
		m_currentState = new WalkState();
		m_stateType = WALK_STATE;
	} else {

	}
}

void MovableBoid::findFoodStateHandler() {

}

bool operator==(const MovableBoid& b1, const MovableBoid& b2)
{
	return b1.getLocation() == b2.getLocation();
}

bool operator!=(const MovableBoid& b1, const MovableBoid& b2)
{
	return !(b1 == b2);
}
