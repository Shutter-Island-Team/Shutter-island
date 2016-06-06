#include <iostream>
#include <cmath>
#include "../../include/Utils.hpp"
#include "../../include/boids2D/StateType.hpp"
#include "../../include/boids2D/MovableBoid.hpp"

MovableBoid::MovableBoid(glm::vec3 location, BoidType t, MovableParametersPtr parameters) 
	: MovableBoid(location, glm::vec3(0,0,0), t, parameters)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, BoidType t, MovableParametersPtr parameters) 
	: MovableBoid(location, velocity, 0.05f, t, parameters)
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    BoidType t, MovableParametersPtr parameters) :
    MovableBoid(location, velocity, mass, t, parameters, 1) 
{

}

MovableBoid::MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    BoidType t, MovableParametersPtr parameters, int amountFood)
	: Boid(location, t, amountFood), m_velocity(velocity), 
	m_acceleration(glm::vec3(0,0,0)), m_mass(mass),
	m_parameters(parameters), m_movablePrey((MovableBoidPtr) nullptr),
	m_rootedPrey((RootedBoidPtr) nullptr), m_hunter((MovableBoidPtr) nullptr),
	m_isDead(false), m_waterTarget(glm::vec3(0,0,2.0f))
{
	#ifdef DEBUG 
    	m_stateType = WALK_STATE;
    #else
    	m_stateType = WALK_STATE;
    #endif

	switch(m_stateType) {
		case TEST_STATE:
			m_currentState.reset(new TestState());
			break;
		case WALK_STATE:
			m_currentState.reset(new WalkState());
			break;
		default:
			std::cerr << "State not recognize. Default state walk initialization" << std::endl;
			m_currentState.reset(new WalkState());
			break;
	}

	switch(t) {
		case RABBIT:
			m_predator = WOLF;
			break;
		case WOLF:
			m_predator = UNKNOWN;
			break;
		default:
			m_predator = UNKNOWN;
			break;
	}
}

glm::vec3 MovableBoid::getVelocity() const
{
	return m_velocity;
}

float MovableBoid::getMass() const
{
	return m_mass;
}

MovableParametersPtr MovableBoid::getParameters() const
{
	return m_parameters;
}

StateType MovableBoid::getStateType() const
{
	return m_stateType;
}

void MovableBoid::resetAcceleration()
{
	m_acceleration = glm::vec3(0, 0, 0);
}

void MovableBoid::computeAcceleration (const BoidsManager & boidsManager, const float & dt, const bool & updateTick)
{
	if(isDead()) {
		switchToState(DEAD_STATE, boidsManager);
		bodyDecomposition();
	} else {
		updateDeadStatus();
	}
	switch (m_stateType) {
		case WALK_STATE:
			walkStateHandler(boidsManager);
			break;
		case STAY_STATE:
			stayStateHandler(boidsManager);
			break;
		case FIND_FOOD_STATE:
			findFoodStateHandler(boidsManager);
			break;
		case ATTACK_STATE:
			attackStateHandler(boidsManager);
			break;
		case EAT_STATE:
			eatStateHandler(boidsManager);
			break;
		case LOST_STATE:
			lostStateHandler(boidsManager);
			break;
		case SLEEP_STATE:
			sleepStateHandler(boidsManager);
			break;
		case FLEE_STATE:
			fleeStateHandler(boidsManager);
			break;
		case FIND_WATER_STATE:
			findWaterStateHandler(boidsManager);
			break;
		case DRINK_STATE:
			drinkStateHandler(boidsManager);
			break;
		case MATE_STATE:
			mateStateHandler(boidsManager);
			break;
		case DEAD_STATE:
			break;
		default:
			std::cerr << "Unknown state" << std::endl;
			break;
	}
	unsigned int i;
	unsigned int j;
	boidsManager.coordToBox(m_location, i, j);
	m_acceleration = m_currentState->computeAcceleration(*this, boidsManager, dt, i, j, updateTick);
}

// x(t + dt) = x(t) + v(t+dt) * dt
const glm::vec3 MovableBoid::computeNextStep(const float & dt)
{
	glm::vec3 prevLocation = m_location;
	glm::vec3 nextVelocity = m_velocity + (dt / m_mass) * limitVec3(m_acceleration, getParameters()->getMaxForce());
	if (m_stateType == FLEE_STATE || m_stateType == ATTACK_STATE) {
		nextVelocity = limitVec3(nextVelocity, getParameters()->getMaxSpeedRun());
	} else {
		nextVelocity = limitVec3(nextVelocity, getParameters()->getMaxSpeedWalk());
	}
  	float k = 0.5f;
  	if(glm::length(m_velocity) < FLT_EPSILON) {
  		m_velocity = 0.015f * nextVelocity + (1.0f - 0.015f) * m_velocity;
  	} else {
  		m_velocity = k * nextVelocity + (1.0f - k) * m_velocity;
  	}
	setAngle(atan2(m_velocity.y, m_velocity.x));
    m_location += dt * m_velocity;
    return prevLocation;
}

bool MovableBoid::canSee(const Boid & other, const float & distView) const
{
	return (distVision(other, distView)) && (angleVision(other) && &other != this);
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

	if (getParameters()->getAngleView() <= M_PI) {
		return (0 <= comparativeValue) && (comparativeValue <= getParameters()->getAngleView()/2);
	} else {
        diffPos = - diffPos;
        comparativeValue = - comparativeValue;
        return !((0 <= comparativeValue) && (comparativeValue <= M_PI - getParameters()->getAngleView()/2));
    }
}

void MovableBoid::switchToState(const StateType & stateType, const BoidsManager & boidsManager) 
{
	switch(stateType) {
		case WALK_STATE:
			m_currentState.reset(new WalkState());
			break;
		case STAY_STATE:
			m_currentState.reset(new StayState());
			break;
		case SLEEP_STATE:
			m_currentState.reset(new SleepState());
			break;
		case FLEE_STATE:
			m_currentState.reset(new FleeState());
			break;
		case FIND_FOOD_STATE:
			m_currentState.reset(new FindFoodState());
			break;
		case ATTACK_STATE:
			m_currentState.reset(new AttackState());
			break;
		case EAT_STATE:
			m_currentState.reset(new EatState());
			break;
		case FIND_WATER_STATE:
			updateWaterTarget(boidsManager);
			m_currentState.reset(new FindWaterState());
			break;
		case DRINK_STATE:
			m_currentState.reset(new DrinkState());
			break;
		case MATE_STATE:
			m_currentState.reset(new MateState());
			break;
		case LOST_STATE:
			m_currentState.reset(new LostState());
			break;
		case DEAD_STATE:
			m_currentState.reset(new DeadState()); 
		case TEST_STATE:
			break;
		default:
			std::cerr << "Unknown state : " << stateType << std::endl;
			break;
	}
	m_stateType = stateType;
}

void MovableBoid::walkStateHandler(const BoidsManager & boidsManager)
{
	if (m_parameters->isInDanger()) {
		switchToState(FLEE_STATE, boidsManager);
	} else if (m_parameters->isThirsty()) {
		switchToState(FIND_WATER_STATE, boidsManager);
	} else if (m_parameters->isHungry()) {
		switchToState(FIND_FOOD_STATE, boidsManager);
	} else if (m_parameters->isTired()) {
		switchToState(STAY_STATE, boidsManager);
	} else {
		return; //Stay in walk state
	}
}

void MovableBoid::stayStateHandler(const BoidsManager & boidsManager)
{

	if (m_parameters->isInDanger()) {
		switchToState(FLEE_STATE, boidsManager);
	} else if (m_parameters->isThirsty()) {
		switchToState(FIND_WATER_STATE, boidsManager);

	} else if (m_parameters->isStarving()) {
		switchToState(FIND_FOOD_STATE, boidsManager);		
	} else if (!m_parameters->isNotTired() && isNight()) {
		switchToState(SLEEP_STATE, boidsManager);
	} else if (!m_parameters->isNotTired()) { 
		// Trick to refill stamina up to high stamina
		return; // Stay in the state
	} else if (hasSoulMate()) {
		switchToState(MATE_STATE, boidsManager);
	} else if (hasLeader() && m_parameters->isHighStamina()) {
		switchToState(WALK_STATE, boidsManager);
	} else if (!hasLeader()) {
		switchToState(LOST_STATE, boidsManager);
	} else {
		return; // Stay in stay state
	}
}

void MovableBoid::findFoodStateHandler(const BoidsManager & boidsManager)
{
	if (m_parameters->isInDanger()) {
		switchToState(FLEE_STATE, boidsManager);
	} else if (hasPrey()) {
		switchToState(ATTACK_STATE, boidsManager);
	} else if (m_parameters->isThirsty()) {
		// If the boid don't find food for a long time it might be better to find water
		switchToState(FIND_WATER_STATE, boidsManager); 
	} else if (m_parameters->isStarving()) {
		return;
	} else if (m_parameters->isTired()) {
		// If the boid don't find food for a long time it might be better to refill stamina
		switchToState(STAY_STATE, boidsManager);
	} else {
		return;
	}
}

void MovableBoid::attackStateHandler(const BoidsManager & boidsManager)
{
	if (m_parameters->isInDanger()) {
		switchToState(FLEE_STATE, boidsManager);
	} else if (m_movablePrey != nullptr && !canSee(*m_movablePrey, m_parameters->getDistViewMax())) {
		m_movablePrey = nullptr;
		switchToState(FIND_FOOD_STATE, boidsManager);
	} else if (closeToPrey()) {
		switchToState(EAT_STATE, boidsManager);
		if(m_movablePrey != (MovableBoidPtr)nullptr) {
			m_movablePrey->die();
		}
	}
}

void MovableBoid::eatStateHandler(const BoidsManager & boidsManager)
{
	if(m_parameters->isInDanger()) {
		switchToState(FLEE_STATE, boidsManager);
	} else if (m_rootedPrey != nullptr && !(m_rootedPrey->isFoodRemaining())) {
		m_rootedPrey = nullptr;
		switchToState(LOST_STATE, boidsManager);
	} else if (m_movablePrey != nullptr && !(m_movablePrey->isFoodRemaining())) {
		m_movablePrey = nullptr;
		switchToState(LOST_STATE, boidsManager);
	} else if (m_parameters->isNotHungry()) {
		if (m_rootedPrey != (RootedBoidPtr) nullptr) {
			m_rootedPrey->decreaseFoodRemaining();
			m_rootedPrey = nullptr;
		} else if (m_movablePrey != (MovableBoidPtr) nullptr) {
			m_movablePrey->decreaseFoodRemaining();
			m_movablePrey = nullptr;
		}
		// Update not in group if he can't see the leader	
		switchToState(LOST_STATE, boidsManager);
	}
}


void MovableBoid::lostStateHandler(const BoidsManager & boidsManager)
{
	if (isInGroup()) {
		switchToState(WALK_STATE, boidsManager);
	} else if (isNight()) {
		switchToState(SLEEP_STATE, boidsManager);
	}
}

void MovableBoid::sleepStateHandler(const BoidsManager & boidsManager)
{
	if (m_parameters->isNotTired()) {
		switchToState(WALK_STATE, boidsManager);
	}
}

void MovableBoid::fleeStateHandler(const BoidsManager & boidsManager)
{
	if (m_parameters->isNotInDanger()) {
		switchToState(LOST_STATE, boidsManager);
	}
}

void MovableBoid::findWaterStateHandler(const BoidsManager & boidsManager)
{
	if (m_parameters->isInDanger()) {
		switchToState(FLEE_STATE, boidsManager);
	} else if (nextToWater(boidsManager)) {
		switchToState(DRINK_STATE, boidsManager);
	}
}

void MovableBoid::drinkStateHandler(const BoidsManager & boidsManager)
{
	if (m_parameters->isNotThirsty()) {
		switchToState(LOST_STATE, boidsManager);
	}
}

void MovableBoid::mateStateHandler(const BoidsManager & boidsManager)
{
	if (isNoLongerMating()) {
		switchToState(SLEEP_STATE, boidsManager);
	}
}

bool MovableBoid::hasPrey() const
{
	return m_movablePrey != (MovableBoidPtr) nullptr || m_rootedPrey != (RootedBoidPtr) nullptr;
}

bool MovableBoid::closeToPrey() const
{
	bool res = false;
	if(m_movablePrey != (MovableBoidPtr) nullptr) {
		res = glm::distance(getLocation(), getMovablePrey()->getLocation()) <= m_parameters->getDistAttack();
	} else if (m_rootedPrey != (RootedBoidPtr) nullptr) {
		res = glm::distance(getLocation(), getRootedPrey()->getLocation()) <= m_parameters->getDistAttack();
	} 
	return res;
}

bool MovableBoid::nextToWater(const BoidsManager & boidsManager) const
{
	glm::vec3 posAhead = getLocation() + cNormalize(getVelocity()) * m_parameters->getDistSeeAhead();
	return boidsManager.getBiome(posAhead.x, posAhead.y) == Lake;
}

bool MovableBoid::hasSoulMate() const
{
	///< @todo
	return false;
}

bool MovableBoid::isNoLongerMating() const
{
	///< @todo
	return false;
}

bool MovableBoid::preyIsDead() const
{
	return (m_movablePrey != (MovableBoidPtr) nullptr) && m_movablePrey->isDead();
}

bool MovableBoid::isNight() const
{
	///< @todo
	return false;
}

bool MovableBoid::isLeader() const
{
	return *m_leader == *this;
}

MovableBoidPtr MovableBoid::getLeader() const
{
	return m_leader;
}

void MovableBoid::setNewLeader(MovableBoidPtr newLeader)
{
	float scale = (*newLeader == *this) ? 2.0f : 1.0f;
	setScale(scale);
	m_leader = newLeader;
}

bool MovableBoid::hasLeader() const
{
	return getLeader() != (MovableBoidPtr) nullptr 
			&& !isLeader();
}

bool MovableBoid::isInGroup() const
{
	return !(getLeader() == nullptr);
}

void MovableBoid::setMovablePrey(const MovableBoidPtr & boid)
{
	m_movablePrey = boid;
	// boid->setHunter(this);
}

MovableBoidPtr MovableBoid::getMovablePrey() const
{
	return m_movablePrey;
}

void MovableBoid::setRootedPrey(const RootedBoidPtr & boid)
{
	m_rootedPrey = boid;
}

RootedBoidPtr MovableBoid::getRootedPrey() const
{
	return m_rootedPrey;
}

void MovableBoid::setHunter(const MovableBoidPtr & boid)
{
	m_hunter = boid;
}

MovableBoidPtr MovableBoid::getHunter() const
{
	return m_hunter;
}

BoidType MovableBoid::getPredatorType() const
{
	return m_predator;
}

bool MovableBoid::isDead() const
{
	return m_isDead;
}

void MovableBoid::die()
{
	m_isDead = true;
}

void MovableBoid::updateDeadStatus()
{
	m_isDead = (m_parameters->getStamina() == 0.0f 
		&& m_parameters->getHunger() == 0.0f 
		&& m_parameters->getThirst() == 0.0f);
}

void MovableBoid::updateWaterTarget(const BoidsManager & b)
{
	glm::vec2 posBoid(m_location.x, m_location.y);
	glm::vec2 result(0,0);
	b.getNearestLake(posBoid, result);
	m_waterTarget = glm::vec3(result.x, result.y, 2.0f);
}

glm::vec3 MovableBoid::getWaterTarget() const
{
	return m_waterTarget;
}

bool operator==(const MovableBoid& b1, const MovableBoid& b2)
{
	return b1.getLocation() == b2.getLocation();
}

bool operator!=(const MovableBoid& b1, const MovableBoid& b2)
{
	return !(b1 == b2);
}
