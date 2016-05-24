#include <iostream>
#include <cmath>

#include "../../include/boids2D/MovableBoid.hpp"

#include "../../include/Utils.hpp"

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

MovableParameters & MovableBoid::getParameters() const {
	return *m_parameters;
}

void MovableBoid::resetAcceleration() {
	m_acceleration = glm::vec3(0, 0, 0);
}

void MovableBoid::computeAcceleration (std::vector<MovableBoidPtr> mvB) {
	switch (m_stateType) {
		case WALK_STATE:
			walkStateHandler(mvB);
			break;
		case STAY_STATE:
			stayStateHandler(mvB);
			break;
		case FIND_FOOD_STATE:
			findFoodStateHandler(mvB);
			break;
		default:
			std::cerr << "Unknown state" << std::endl;
			break;
	}
	setAcceleration(m_currentState->computeAcceleration(*this, mvB));
}

void MovableBoid::computeNextStep(float dt) {
    m_velocity = limitVec3(m_velocity + (dt / m_mass) * limitVec3(m_acceleration, getParameters().getMaxForce()), getParameters().getMaxSpeed());
	setAngle(atan2(m_velocity.y, m_velocity.x));
    m_location += dt * m_velocity;
}

bool MovableBoid::canSee(Boid b, float distView) const {
	return (distVision(b, distView)) && (angleVision(b));
}

bool MovableBoid::distVision (Boid b, float distView) const {
	return (glm::distance(m_location, b.getLocation()) < distView);
}

bool MovableBoid::sameSpecies(Boid b)
{
	return b.getBoidType() == getBoidType();
}

bool MovableBoid::angleVision (Boid b) const {
	glm::vec3 diffPos = b.getLocation() - m_location;
	float comparativeValue = acos(glm::dot(glm::normalize(m_velocity), glm::normalize(diffPos)));

	if (getParameters().getAngleView() <= M_PI) {
		return (0 <= comparativeValue) && (comparativeValue <= getParameters().getAngleView()/2);
	} else {
        diffPos = - diffPos;
        comparativeValue = - comparativeValue;
        return !((0 <= comparativeValue) && (comparativeValue <= M_PI - getParameters().getAngleView()/2));
    }
}

void MovableBoid::walkStateHandler(std::vector<MovableBoidPtr> mvB) {
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

void MovableBoid::stayStateHandler(std::vector<MovableBoidPtr> mvB) {
	if (m_parameters->isHighStamina()) {
		delete m_currentState;
		m_currentState = new WalkState();
		m_stateType = WALK_STATE;
	} else {

	}
}

void MovableBoid::findFoodStateHandler(std::vector<MovableBoidPtr> mvB) {

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
