#include "../../include/boids2D/MovableState.hpp"
#include "../../include/terrain/Biome.hpp"
#include "../../include/Utils.hpp"
#include <iostream>

MovableBoidPtr closestMovable(const MovableBoid & b, const BoidType & type, const std::list<MovableBoidPtr> & mvB) {
	float tmpDistance = FLT_MAX;
	MovableBoidPtr target = (MovableBoidPtr) nullptr;

	for (MovableBoidPtr m : mvB) {
		if (b.canSee(*m, b.getParameters()->getDistViewMax()) && b.distVision(*m, tmpDistance) && m->getBoidType() == type) { 
			// TODO : optimization possible		
			tmpDistance = glm::distance(m->getLocation(), b.getLocation());
			target = m;
		}
	}
	return target;
}

RootedBoidPtr closestRooted(const MovableBoid & b, const BoidType & type, const std::list<RootedBoidPtr> & rtB) {
	float tmpDistance = FLT_MAX;
	RootedBoidPtr target = (RootedBoidPtr) nullptr;

	for (RootedBoidPtr r : rtB) {
		if (b.canSee(*r, b.getParameters()->getDistViewMax()) && b.distVision(*r, tmpDistance) && r->getBoidType() == type) { 
			// TODO : optimization possible		
			tmpDistance = glm::distance(r->getLocation(), b.getLocation());
			target = r;
		}
	}
	return target;
}

glm::vec3 MovableState::computeAcceleration(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// Reset acceleration
	b.resetAcceleration();
	return computeNewForces(b, boidsManager, dt, i, j, updateTick);
}

glm::vec3 MovableState::seek(const MovableBoid& b, const glm::vec3 & target) const
{
	glm::vec3 desiredVelocity = target - b.getLocation();
	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= b.getParameters()->getMaxSpeedRun();
	glm::vec3 steer = desiredVelocity - b.getVelocity();

	return steer;
}

glm::vec3 MovableState::flee(const MovableBoid& b, const glm::vec3 & repulsionPoint) const
{
	return -seek(b, repulsionPoint);
}

glm::vec3 MovableState::wander(const MovableBoid& b) const
{

	float randomVal = random(0.0f, 2*M_PI);

	glm::vec3 randomVec3(cos(randomVal), sin(randomVal), 0);

    glm::vec3 desiredTarget = b.getLocation() 
    	+ b.getParameters()->getRadiusCircleWander()*b.getVelocity() 
    	+ b.getParameters()->getRadiusCircleWander()*randomVec3;

    return arrive(b, desiredTarget);
}

// TODO : Maybe reuse seek
glm::vec3 MovableState::arrive(const MovableBoid& b, const glm::vec3 & target) const
{
	glm::vec3 desiredVelocity = target - b.getLocation();

	float distance = glm::length(desiredVelocity);
	desiredVelocity = cNormalize(desiredVelocity);
	if (distance < b.getParameters()->getDistStartSlowingDown()) {
		// Set the magnitude according to how close we are.
		// Linear computation of the magnitude
	  	float magnitude = distance * b.getParameters()->getMaxSpeedRun()
	  		/ b.getParameters()->getDistStartSlowingDown();
	  desiredVelocity *= magnitude;
	} else {
	  // Otherwise, proceed at maximum speed.
	  desiredVelocity *= b.getParameters()->getMaxSpeedRun();
	}

	// The usual steering = desiredVelocity - velocity
	glm::vec3 steer = desiredVelocity - b.getVelocity();
	limitVec3(steer, b.getParameters()->getMaxForce());
	return steer;
}

void MovableState::stop(MovableBoid & b) const
{
	b.resetVelocity();
}

glm::vec3 MovableState::stayWithinWalls(const MovableBoid& b) const
{
	glm::vec3 steer(0, 0, 0);
	float distToWall = 20.0f;
    if (b.getLocation().x < -distToWall) {
    	glm::vec3 desired(b.getParameters()->getMaxForce(), b.getVelocity().y, 0);
    	steer = desired - b.getVelocity();
    } else if (b.getLocation().x >  distToWall) {
		glm::vec3 desired(-b.getParameters()->getMaxForce(), b.getVelocity().y, 0);
		steer = desired - b.getVelocity() ;
    }
    
    if (b.getLocation().y < -distToWall) {
		glm::vec3 desired(b.getVelocity().x, b.getParameters()->getMaxForce(), 0);
		steer += desired - b.getVelocity();
    } else if (b.getLocation().y >  distToWall) {
		glm::vec3 desired(b.getVelocity().x, -b.getParameters()->getMaxForce(), 0);
		steer += desired - b.getVelocity();
    }

    steer = limitVec3(steer, b.getParameters()->getMaxForce());

    return steer;
}

glm::vec3 MovableState::stayOnIsland(const MovableBoid & b, const BoidsManager & boidsManager) const
{
	glm::vec3 posAheadMax = b.getLocation() + cNormalize(b.getVelocity()) * b.getParameters()->getDistViewMax();
	glm::vec3 posAhead = b.getLocation() + cNormalize(b.getVelocity()) * b.getParameters()->getDistSeeAhead();

	if(boidsManager.getBiome(posAheadMax.x, posAheadMax.y) == Sea 
		|| boidsManager.getBiome(b.getLocation().x, b.getLocation().y) == Sea) {
		
		///< TODO move to the center of the map
		glm::vec3 islandCenter(250.0, 250.0, 0.0);

		return cNormalize(islandCenter - b.getLocation()) * b.getParameters()->getMaxForce();
	} else if (b.getStateType() != FIND_WATER_STATE && 
		(boidsManager.getBiome(posAhead.x, posAhead.y) == Lake || 
			boidsManager.getBiome(b.getLocation().x, b.getLocation().y) == Lake)) 
	{

		glm::vec3 lakeCenter(0,0,0);

		bool lakeFound = boidsManager.getMap().getClosestLake(b.getLocation().x, b.getLocation().y, lakeCenter.x, lakeCenter.y);
		if(lakeFound) {
			return cNormalize(b.getLocation() - lakeCenter) * b.getParameters()->getMaxForce();
		} else {
			return glm::vec3(0,0,0); // There is no lake
		}
	} else {
		return glm::vec3(0,0,0);
	}
}

glm::vec3 MovableState::coherentWalk(const MovableBoid & b, const BoidsManager & boidsManager) const
{
	glm::vec3 posAheadMax = b.getLocation() + cNormalize(b.getVelocity()) * b.getParameters()->getDistViewMax();
	glm::vec3 posAhead = b.getLocation() + cNormalize(b.getVelocity()) * b.getParameters()->getDistSeeAhead();

	float heightDifference = boidsManager.getHeight(posAheadMax.x, posAheadMax.y) - boidsManager.getHeight(b.getLocation().x, b.getLocation().y);
	if(heightDifference > -2.0f || heightDifference < -2.0f
		|| boidsManager.getBiome(posAheadMax.x, posAheadMax.y) == Mountain 
		|| boidsManager.getBiome(b.getLocation().x, b.getLocation().y) == Mountain
		|| boidsManager.getBiome(posAheadMax.x, posAheadMax.y) == Peak 
		|| boidsManager.getBiome(b.getLocation().x, b.getLocation().y) == Peak) {
		return cNormalize(- posAheadMax) * b.getParameters()->getMaxForce();
	} else {
		return glm::vec3(0,0,0);
	}
}



glm::vec3 MovableState::separate(const MovableBoid& b, const std::list<MovableBoidPtr> & bVec) const
{
	glm::vec3 sum(0,0,0);
	glm::vec3 steer(0,0,0);
	int count = 0;
	for(BoidPtr bElt : bVec) {
		float d = glm::distance(b.getLocation(), bElt->getLocation());
		if ((d > 0) && b.distVision(*bElt, b.getParameters()->getDistSeparate())) {
			glm::vec3 diff = b.getLocation() - bElt->getLocation();
			diff = glm::normalize(diff);
			sum += diff;
			count++;
		}
	}
	if (count > 0) {
		sum /= count;
		sum = glm::normalize(sum) * b.getParameters()->getMaxSpeedRun();
		steer = sum - b.getVelocity();
		steer = limitVec3(steer, b.getParameters()->getMaxForce());
	}
	return steer;
}

glm::vec3 MovableState::collisionAvoid (const MovableBoid& b, const std::list<RootedBoidPtr> & rootB) const
{
	float coeff = glm::length(b.getVelocity()) / b.getParameters()->getMaxSpeedRun();
	glm::vec3 posAhead = b.getLocation() + coeff * cNormalize(b.getVelocity()) * b.getParameters()->getDistSeeAhead();
	glm::vec3 posAhead2 = b.getLocation() + coeff *  cNormalize(b.getVelocity()) * b.getParameters()->getDistSeeAhead() * 0.5f;
	std::list<RootedBoidPtr>::const_iterator it = rootB.begin();
	RootedBoidPtr eltFar = (RootedBoidPtr) nullptr;
	RootedBoidPtr eltClose = (RootedBoidPtr) nullptr;
	RootedBoidPtr elt;
	while (it != rootB.end() && eltClose == (RootedBoidPtr) nullptr) {
		elt = *it;
		if (glm::distance(elt->getLocation(), posAhead) <= elt->getRadius()) {
			eltFar = elt;
		} else if (glm::distance(elt->getLocation(), posAhead2) <= elt->getRadius()) {
			eltClose = elt;
		}
		it++;
	}
	if (!(eltClose == (RootedBoidPtr) nullptr)) {
		return glm::normalize(posAhead - eltClose->getLocation()) * b.getParameters()->getMaxForce();
	} else if (!(eltFar == (RootedBoidPtr) nullptr)) {
		return glm::normalize(posAhead - eltFar->getLocation()) * b.getParameters()->getMaxForce();
	} else {
		return glm::vec3(0,0,0);
	}
}

glm::vec3 MovableState::align (const MovableBoid& b, const std::list<MovableBoidPtr> & mvB) const
{
	glm::vec3 sum(0,0,0);
	glm::vec3 steer;
	int count = 0;
	for (MovableBoidPtr other : mvB) {
		if(b.sameSpecies(*other)) 
		{
			float d = glm::distance(b.getLocation(), other->getLocation());
			if ((d > 0) && (d < b.getParameters()->getDistViewCohesion()) && b.canSee(*other, b.getParameters()->getAngleView())) 
			{
				sum += other->getVelocity();
				// For an average, we need to keep track of
				// how many boids are within the distance.
				count++;
			}
		}
	}

	if (count > 0) {
		sum = sum / (float) count;
		sum = cNormalize(sum); ///< @todo : is it normal ?
		sum *= b.getParameters()->getMaxForce();
		steer = sum - b.getVelocity();
		steer = limitVec3(steer, b.getParameters()->getMaxForce());
	} else {
		steer = glm::vec3(0,0,0);
	}

	return steer;
}

glm::vec3 MovableState::cohesion (const MovableBoid & b, const std::list<MovableBoidPtr> & mvB) const
{
    glm::vec3 sum(0,0,0);
    glm::vec3 steer;
    int count = 0;
    for (MovableBoidPtr other : mvB) {
    	if(b.sameSpecies(*other)) 
		{
			float d = glm::distance(b.getLocation(), other->getLocation());
			if ((d > 0) && (d < b.getParameters()->getDistViewCohesion()) && b.canSee(*other, b.getParameters()->getAngleView())) {
				// Adding up all the others’ locations
				sum += other->getLocation();
				count++;
			}
		}
    }
    if (count > 0) {
		sum /= (float) count;
		return seek(b, sum);
    } else {
    	return glm::vec3(0,0,0);
    }
}

// xForecast(t) = x(t) + v(t) * dt
glm::vec3 MovableState::positionForecast(const MovableBoid & b, const float & dt, const float & cst) const
{
	return b.getLocation() + dt * b.getVelocity() * cst;
}

glm::vec3 MovableState::pursuit (const MovableBoid & hunter, const MovableBoid & target, const float & dt) const
{
	const float val = glm::distance(hunter.getLocation(), target.getLocation())
									/ (hunter.getParameters()->getMaxSpeedRun());
	return seek(hunter, positionForecast(target, dt, val));
}

glm::vec3 MovableState::evade(const MovableBoid & prey, const MovableBoid & hunter, const float & dt) const
{
	if (&prey == nullptr) {
		throw std::invalid_argument("Prey is null in evade");
	}
	if (&hunter == nullptr) {
		throw std::invalid_argument("Hunter is null in evade");
	}
	const float val = glm::distance(prey.getLocation(), hunter.getLocation())
									/ (hunter.getParameters()->getMaxSpeedRun());
	return flee(prey, positionForecast(hunter, dt, val));
}

// Precondition b.hasLeader() == true
glm::vec3 MovableState::followLeader(const MovableBoid & b, const std::list<MovableBoidPtr> & mvB, const float & dt,
	const float & separateCoeff, const float & evadeCoeff) const
{
	glm::vec3 steer(0,0,0);
	MovableBoidPtr leader = b.getLeader();
	if (leader->getVelocity() == glm::vec3(0,0,0)) { // Trick to avoid error with normalize when leader->getVelocity() == (0,0,0)
		return glm::vec3(0, 0, 0); // Don't compute any force, it is fine
	}
	glm::vec3 positionBehindLeader = leader->getLocation() + glm::normalize(-1.0f * leader->getVelocity()) * b.getParameters()->getDistToLeader();
	steer = arrive(b, positionBehindLeader);
	steer += separateCoeff * separate(b, mvB); // Coefficient can be modify
	if (leader->canSee(b, 1.4f * leader->getParameters()->getDistSeparate())) { // Can be modify
		steer += evadeCoeff * evade(b, *leader, dt);
	}
	limitVec3(steer, b.getParameters()->getMaxForce());
	return steer;	
}

void MovableState::updateDanger(MovableBoid& b, const std::list<MovableBoidPtr> & mvB) const
{
	BoidType predator = b.getPredatorType();
	bool predatorFound = false;
	MovableBoidPtr predatorBoid;

	std::list<MovableBoidPtr>::const_iterator it = mvB.begin();
	while(!predatorFound && it != mvB.end()) {
		if((*it)->getBoidType() == predator && b.canSee(**it, b.getParameters()->getDistViewMax())) {
			predatorBoid = *it;
			predatorFound = true;
		}
		++it;
	}

	if(predatorFound && !predatorBoid->isDead()) {
		b.getParameters()->dangerIncrease();
	} else {
		b.getParameters()->dangerDecrease();
	}
}

void MovableState::updateAffinity(MovableBoid& b, const std::list<MovableBoidPtr> & mvB) const
{
	bool friendFound = false;

	std::list<MovableBoidPtr>::const_iterator it = mvB.begin();
	while(!friendFound && it != mvB.end()) {
		if(b != **it && (*it)->getBoidType() == b.getBoidType() && b.canSee(**it, b.getParameters()->getDistViewMax())) {
			friendFound = true;
		}
		++it;
	}

	if(friendFound) {
		b.getParameters()->affinityIncrease();
	} else {
		b.getParameters()->affinityDecrease();
	}
}

glm::vec3 MovableState::globalAvoid(const MovableBoid & b, const BoidsManager & boidsManager, const int & i, const int & j, const float & dt) const
{
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);
	glm::vec3 avoid = boidsManager.m_forceController.getSeparate() * separate(b, mvB)
		+ boidsManager.m_forceController.getCohesion() * cohesion(b, mvB)
		+ boidsManager.m_forceController.getAlign() * align(b, mvB) 
		+ avoidEnvironment(b, boidsManager, i, j);

	if(b.getLeader()->canSee(b, 1.4f * b.getLeader()->getParameters()->getDistSeparate())) {
		avoid += boidsManager.m_forceController.getEvade() * evade(b, *b.getLeader(), dt);
	}
	return avoid;
}

glm::vec3 MovableState::avoidEnvironment(const MovableBoid & b, const BoidsManager & boidsManager, const int & i, const int & j) const
{
	return boidsManager.m_forceController.getStayOnIsland() * stayOnIsland(b, boidsManager) 
			//+ boidsManager.m_forceController.getStayOnIsland() * coherentWalk(b, boidsManager)
			+ boidsManager.m_forceController.getCollisionAvoidance() * collisionAvoid(b, boidsManager.getRootedBoids(i, j));
}

/* ==================================== Boid State Value ====================================
 * Each States update stamina, hunger, thrist, danger and affinity
 * Increasing function are si(stamina), hi(hunger), ti(thrist), di(danger), ai(affinity)
 * Decreasing function are sd(stamina), hd(hunger), td(thrist), dd(danger), ad(affinity)
 * These function are define in [0, 100]
 */

/* 
 * State priority : FleeState, FindFood, Eat, Stay, Walk, ...
 * 
 */

glm::vec3 WalkState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	glm::vec3 newForces(0,0,0);
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);

	if (updateTick) {
		b.getParameters()->staminaDecreaseWalk();
		b.getParameters()->hungerDecreaseWalk();
		b.getParameters()->thirstDecreaseWalk();
	}

	// Detect danger and update danger parameter 
	updateDanger(b, mvB);

	// Detect if alone and update affinity
	updateAffinity(b, mvB);

	if(b != *b.getLeader()) { // Follower of the leader
		newForces = boidsManager.m_forceController.getFollowLeader() * followLeader(b, mvB, dt,
			boidsManager.m_forceController.getSeparate(),
			boidsManager.m_forceController.getEvade())
			+ avoidEnvironment(b, boidsManager, i, j);
	} else  { // Only leader
		newForces = wander(b) + globalAvoid(b, boidsManager, i, j, dt); //avoidEnvironment(b, boidsManager, i, j);
	}

	newForces.z = 0.0f; // Trick to compute force in 2D
	return newForces;
}

glm::vec3 StayState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// /!\ requirement : danger <= lowDanger
	// stamina <- si(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);

	if (updateTick) {
		b.getParameters()->staminaIncrease();
		b.getParameters()->hungerDecreaseWalk();
		b.getParameters()->thirstDecreaseWalk();
	}

	// Detect danger and update danger parameter 
	updateDanger(b, mvB);

	// Detect if alone and update affinity
	updateAffinity(b, mvB);
	stop(b);

	return glm::vec3(0,0,0);
}

glm::vec3 SleepState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// /!\ requirement : danger <= lowDanger
	// stamina <- si(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// danger did'nt change the boid is not aware of his surronding
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	glm::vec3 newForces(0,0,0);
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);

	if (updateTick) {
		b.getParameters()->staminaIncrease();
		b.getParameters()->hungerDecreaseWalk();
		b.getParameters()->thirstDecreaseWalk();
	}

	// Detect if alone and update affinity
	updateAffinity(b, mvB);

	newForces += arrive(b, b.getLocation());
	newForces.z = 0.0f; // Trick to compute force in 2D

	return newForces;
}

glm::vec3 FleeState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// TODO : velocity <- max_speed in the opposite direction of predators
	// stamina <- sd(stamina) 
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity) 
	glm::vec3 newForces(0,0,0);
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);

	if (updateTick) {
		b.getParameters()->staminaDecreaseRun();
		b.getParameters()->hungerDecreaseRun();
		b.getParameters()->thirstDecreaseRun();
	}

	// Detect danger and update danger parameter 
	updateDanger(b, mvB);

	// Detect if alone and update affinity
	updateAffinity(b, mvB);

	MovableBoidPtr rabbitPredator;
	switch (b.getBoidType()) {
		case WOLF:
			// nothing to do has no predator
		break;
		case RABBIT:
			rabbitPredator = closestMovable(b, b.getPredatorType(), mvB);
			if (rabbitPredator == (MovableBoidPtr) nullptr) {
				newForces += wander(b);
			} else {
				if(!rabbitPredator->isDead()) {
					b.setHunter(rabbitPredator);
					newForces += evade(b, *b.getHunter(), dt);
				}
			}
			break;
		default:
			std::cerr << "Unknown animal looking for food" << std::endl;
			newForces += glm::vec3(0,0,0);
			break;
	}

	newForces += globalAvoid(b, boidsManager, i, j, dt);
	newForces.z = 0.0f; // Trick to compute force in 2D
	return newForces;
}

glm::vec3 FindFoodState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// TODO : wander or follow group until the boid find sth (we can mix both in funtion of hunger variable)
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	glm::vec3 newForces(0,0,0);
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);
	const std::list<RootedBoidPtr> rtB = boidsManager.getRootedBoids(i, j);

	if (updateTick) {
		b.getParameters()->staminaDecreaseWalk();
		b.getParameters()->hungerDecreaseWalk();
		b.getParameters()->thirstDecreaseWalk();
	}

	// Detect danger and update danger parameter 
	updateDanger(b, mvB);

	// Detect if alone and update affinity
	updateAffinity(b, mvB);

	MovableBoidPtr movableTarget;
	RootedBoidPtr rootedTarget;
	switch (b.getBoidType()) {
		case WOLF:
			movableTarget = closestMovable(b, RABBIT, mvB);
			if (movableTarget == (MovableBoidPtr) nullptr) {
				newForces += wander(b);
			} else {
				b.setMovablePrey(movableTarget);
				newForces += pursuit(b, *movableTarget, dt);
			}
			break;
		case RABBIT:
			rootedTarget = closestRooted(b, CARROT, rtB);
			if (rootedTarget == (RootedBoidPtr) nullptr) {
				newForces += wander(b);
			} else {
				b.setRootedPrey(rootedTarget);
				newForces += arrive(b, rootedTarget->getLocation());
			}
			break;
		default:
			std::cerr << "Unknown animal looking for food" << std::endl;
			newForces += glm::vec3(0,0,0);
			break;
	}
	newForces += globalAvoid(b, boidsManager, i, j, dt);
	newForces.z = 0.0f; // Trick to compute force in 2D
	return newForces;
}

glm::vec3 EatState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// stamina <- si(stamina)
	// hunger <- hi(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);

	if (updateTick) {
		b.getParameters()->staminaIncrease();
		b.getParameters()->hungerIncrease();
		b.getParameters()->thirstDecreaseWalk();
	}

	// Detect danger and update danger parameter 
	updateDanger(b, mvB);

	// Detect if alone and update affinity
	updateAffinity(b, mvB);

	stop(b);
	return glm::vec3(0,0,0);
}

glm::vec3 FindWaterState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// TODO : wander or follow group until the boid find sth (we can mix both in funtion of thirst variable)
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	glm::vec3 newForces(0,0,0);
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);

	if (updateTick) {
		b.getParameters()->staminaDecreaseWalk();
		b.getParameters()->hungerDecreaseWalk();
		b.getParameters()->thirstDecreaseWalk();
	}

	// Detect danger and update danger parameter 
	updateDanger(b, mvB);

	// Detect if alone and update affinity
	updateAffinity(b, mvB);

	newForces += arrive(b, b.getWaterTarget());
	newForces += globalAvoid(b, boidsManager, i, j, dt);
	newForces.z = 0.0f; // Trick to compute force in 2D
	return newForces;
}

glm::vec3 DrinkState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// TODO : velocity <- (0,0,0)
	// stamina <- si(stamina)
	// hunger <- hd(hunger)
	// thirst <- ti(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	glm::vec3 newForces(0,0,0);
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);
	
	if (updateTick) {
		b.getParameters()->staminaIncrease();
		b.getParameters()->hungerDecreaseWalk();
		b.getParameters()->thirstIncrease();
	}

	// Detect danger and update danger parameter 
	updateDanger(b, mvB);

	// Detect if alone and update affinity
	updateAffinity(b, mvB);

	stop(b);

	return glm::vec3(0,0,0);
}

glm::vec3 MateState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// TODO : velocity <- (0,0,0) && create a new boid
	// requirement : danger <= lowDanger
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);

	MovableBoidPtr mate = closestMovable(b, b.getBoidType(), mvB);
	
	if (updateTick) {
		b.getParameters()->staminaDecreaseWalk();
		b.getParameters()->hungerDecreaseWalk();
		b.getParameters()->thirstDecreaseWalk();
	}

	// Detect danger and update danger parameter 
	updateDanger(b, mvB);

	// Set affinity to 0.0 to limit the number of birth
	stop(b);
	return glm::vec3(0,0,0);
}

glm::vec3 AttackState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// TODO : velocity <- max velocity to taget
	// stamina <- decrease
	// hunger <- decrease
	// thirst <- decrease
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if alone affinity <- ad(affinity)
	glm::vec3 newForces(0,0,0);
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);

	if (updateTick) {
		b.getParameters()->staminaDecreaseRun();
		b.getParameters()->hungerDecreaseRun();
		b.getParameters()->thirstDecreaseRun();
	}

	// Detect danger and update danger parameter 
	updateDanger(b, mvB);

	// Detect if alone and update affinity
	updateAffinity(b, mvB);

	switch (b.getBoidType()) {
		case WOLF:
			newForces += pursuit(b, *b.getMovablePrey(), dt);
			break;
		case RABBIT:
			newForces += arrive(b, b.getRootedPrey()->getLocation());
			break;
		default:
			std::cerr << "Unknown animal looking for food" << std::endl;
			newForces += glm::vec3(0,0,0);
			break;
	}
	newForces += avoidEnvironment(b, boidsManager, i, j);
	newForces += separate(b, mvB);
	newForces.z = 0.0f; // Trick to compute force in 2D

	return newForces;
}

glm::vec3 LostState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	const std::list<MovableBoidPtr> mvB = boidsManager.getNeighbour(i, j);
	
	// Update boid status parameters
	if (updateTick) {
		b.getParameters()->staminaDecreaseWalk();
		b.getParameters()->hungerDecreaseWalk();
		b.getParameters()->thirstDecreaseWalk();
	}
	
	// Detect danger and update danger parameter 
	updateDanger(b, mvB);

	// Detect if alone and update affinity
	updateAffinity(b, mvB);

	if (glm::length(b.getVelocity()) < 0.2f && glm::distance(b.getLocation(), b.getLandmarkPosition()) < 5.0f) {
		stop(b);
		return glm::vec3(0,0,0);
	} else {
		// wander and avoid obstacle until the boid need to eat or find a group
		glm::vec3 newForces = arrive(b, b.getLandmarkPosition()) + globalAvoid(b, boidsManager, i, j, dt); 
		newForces.z = 0.0f; // Trick to compute force in 2D
		return newForces;
	}
}

glm::vec3 DeadState::computeNewForces(MovableBoid& b, const BoidsManager & boidsManager, const float & dt, const int & i, const int & j, const bool & updateTick) const
{
	// Don't move because the boid is dead
	glm::vec3 newForces(0,0,0);

	newForces += arrive(b, b.getLocation());
	newForces.z = 0.0f; // Trick to compute force in 2D
	return newForces;
}
