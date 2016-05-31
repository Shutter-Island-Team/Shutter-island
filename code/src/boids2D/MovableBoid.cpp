#include <iostream>
#include <cmath>
#include "../../include/Utils.hpp"
#include "../../include/boids2D/StateType.hpp"
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
	m_parameters(parameters), m_movablePrey((MovableBoidPtr) nullptr),
	m_rootedPrey((RootedBoidPtr) nullptr), m_hunter((MovableBoidPtr) nullptr),
	m_isDead(false)
{
	#ifdef DEBUG 
    	m_stateType = WALK_STATE;
    #else
    	m_stateType = WALK_STATE;
    #endif

	switch(m_stateType) {
		case TEST_STATE:
			m_currentState = new TestState();
			break;
		case WALK_STATE:
			m_currentState = new WalkState();
			break;
		default:
			std::cerr << "State not recognize. Default state walk initialization" << std::endl;
			m_currentState = new WalkState();
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

MovableParameters & MovableBoid::getParameters() const
{
	return *m_parameters;
}

StateType MovableBoid::getStateType() const
{
	return m_stateType;
}

void MovableBoid::resetAcceleration()
{
	m_acceleration = glm::vec3(0, 0, 0);
}

void MovableBoid::computeAcceleration (const BoidsManager & boidsManager, const float & dt)
{
	if(isDead()) {
		switchToState(DEAD_STATE);
	} else {
		updateDeadStatus();
	}
	switch (m_stateType) {
		case TEST_STATE:
			break;
		case WALK_STATE:
			walkStateHandler();
			break;
		case STAY_STATE:
			stayStateHandler();
			break;
		case FIND_FOOD_STATE:
			findFoodStateHandler();
			break;
		case ATTACK_STATE:
			attackStateHandler();
			break;
		case EAT_STATE:
			eatStateHandler();
			break;
		case LOST_STATE:
			lostStateHandler();
			break;
		case SLEEP_STATE:
			sleepStateHandler();
			break;
		case FLEE_STATE:
			fleeStateHandler();
			break;
		case FIND_WATER_STATE:
			findWaterStateHandler();
			break;
		case DRINK_STATE:
			drinkStateHandler();
			break;
		case MATE_STATE:
			mateStateHandler();
			break;
		case DEAD_STATE:
			break;
		default:
			std::cerr << "Unknown state" << std::endl;
			break;
	}
	m_acceleration = m_currentState->computeAcceleration(*this, boidsManager, dt);
	std::cerr << "Acceleration :";
	displayVec3(m_acceleration);
	std::cerr << std::endl;
}

// x(t + dt) = x(t) + v(t+dt) * dt
void MovableBoid::computeNextStep(const float & dt)
{
	std::cerr << "Previous velocity :";
	displayVec3(m_velocity);
	std::cerr << std::endl;
    m_velocity = limitVec3(m_velocity + (dt / m_mass) * limitVec3(m_acceleration, getParameters().getMaxForce()), getParameters().getMaxSpeed());
    std::cerr << "Next velocity :";
	displayVec3(m_velocity);
	std::cerr << std::endl;
	setAngle(atan2(m_velocity.y, m_velocity.x));
    m_location += dt * m_velocity;
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

	if (getParameters().getAngleView() <= M_PI) {
		return (0 <= comparativeValue) && (comparativeValue <= getParameters().getAngleView()/2);
	} else {
        diffPos = - diffPos;
        comparativeValue = - comparativeValue;
        return !((0 <= comparativeValue) && (comparativeValue <= M_PI - getParameters().getAngleView()/2));
    }
}

void MovableBoid::switchToState(const StateType & stateType) 
{
	delete m_currentState;
	switch(stateType) {
		case WALK_STATE:
			m_currentState = new WalkState();
			break;
		case STAY_STATE:
			m_currentState = new StayState();
			break;
		case SLEEP_STATE:
			m_currentState = new SleepState();
			break;
		case FLEE_STATE:
			m_currentState = new FleeState();
			break;
		case FIND_FOOD_STATE:
			m_currentState = new FindFoodState();
			break;
		case ATTACK_STATE:
			m_currentState = new AttackState();
			break;
		case EAT_STATE:
			m_currentState = new EatState();
			break;
		case FIND_WATER_STATE:
			m_currentState = new FindWaterState();
			break;
		case DRINK_STATE:
			m_currentState = new DrinkState();
			break;
		case MATE_STATE:
			m_currentState = new MateState();
			break;
		case LOST_STATE:
			m_currentState = new LostState();
			break;
		case DEAD_STATE:
			m_currentState = new DeadState(); 
		case TEST_STATE:
			break;
		default:
			std::cerr << "Unknown state : " << stateType << std::endl;
			break;
	}
	m_stateType = stateType;
}

void MovableBoid::walkStateHandler()
{
	if (m_parameters->isInDanger()) {
		switchToState(FLEE_STATE);
	} else if (m_parameters->isThirsty()) {
		switchToState(FIND_WATER_STATE);
	} else if (m_parameters->isHungry()) {
		switchToState(FIND_FOOD_STATE);
	} else if (m_parameters->isTired()) {
		switchToState(STAY_STATE);
	}
}

void MovableBoid::stayStateHandler()
{

	if (m_parameters->isInDanger()) {
		switchToState(FLEE_STATE);
	} else if (m_parameters->isThirsty()) {
		switchToState(FIND_WATER_STATE);
	} else if (m_parameters->isHungry()) {
		switchToState(FIND_FOOD_STATE);
	} else if (!m_parameters->isNotTired()) {
		return; // Stay in the state
	}else if (isNight()) {
		switchToState(SLEEP_STATE);
	} else if (hasSoulMate()) {
		switchToState(MATE_STATE);
	} else if (hasLeader() && m_parameters->isHighStamina()) {
		switchToState(WALK_STATE);
	} else if (!hasLeader()) {
		switchToState(LOST_STATE);
	}
}

void MovableBoid::findFoodStateHandler()
{
	///< @todo : A modifier
	if (hasPrey()) {
		switchToState(ATTACK_STATE);
	} else if (hasPrey() && preyIsDead()) {
		switchToState(EAT_STATE);
	}
}

void MovableBoid::attackStateHandler()
{
	if (m_parameters->isInDanger()) {
		switchToState(FLEE_STATE);
	} else if (closeToPrey()) {
		switchToState(EAT_STATE);
		if(m_movablePrey != (MovableBoidPtr)nullptr) {
			m_movablePrey->die();
		}
	}
}

void MovableBoid::eatStateHandler()
{
	if(m_parameters->isInDanger()) {
		switchToState(FLEE_STATE);
	} else if(m_parameters->isNotHungry()) {
		if (m_rootedPrey != (RootedBoidPtr) nullptr) {
			m_rootedPrey->disapear();
			setRootedPrey(nullptr);
		}		
		switchToState(LOST_STATE);
	}
}


void MovableBoid::lostStateHandler()
{
	if (isInGroup()) {
		switchToState(WALK_STATE);
	} else if (isNight()) {
		switchToState(SLEEP_STATE);
	}
}

void MovableBoid::sleepStateHandler()
{
	if (m_parameters->isNotTired()) {
		switchToState(WALK_STATE);
	}
}

void MovableBoid::fleeStateHandler()
{
	if (m_parameters->isNotInDanger()) {
		switchToState(LOST_STATE);
	}
}

void MovableBoid::findWaterStateHandler()
{
	if (m_parameters->isInDanger()) {
		switchToState(FLEE_STATE);
	} else if (nextToWater()) {
		switchToState(DRINK_STATE);
	}
}

void MovableBoid::drinkStateHandler()
{
	if (m_parameters->isNotThirsty()) {
		switchToState(LOST_STATE);
	}
}

void MovableBoid::mateStateHandler()
{
	if (isNoLongerMating()) {
		switchToState(SLEEP_STATE);
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

bool MovableBoid::nextToWater() const
{
	///< @todo
	return false;
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

bool MovableBoid::isDead()
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

bool operator==(const MovableBoid& b1, const MovableBoid& b2)
{
	return b1.getLocation() == b2.getLocation();
}

bool operator!=(const MovableBoid& b1, const MovableBoid& b2)
{
	return !(b1 == b2);
}
