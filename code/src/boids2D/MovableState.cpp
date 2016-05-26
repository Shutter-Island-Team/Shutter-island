#include "../../include/boids2D/MovableState.hpp"

#include "../../include/Utils.hpp"
#include <iostream>

glm::vec3 MovableState::computeAcceleration(MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// Reset acceleration
	b.resetAcceleration();
	return computeNewForces(b, boidsManager, dt);
}

glm::vec3 MovableState::seek(const MovableBoid& b, const glm::vec3 & target) const
{
	glm::vec3 desiredVelocity = target - b.getLocation();
	desiredVelocity = glm::normalize(desiredVelocity);
	desiredVelocity *= b.getParameters().getMaxSpeed();
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
    	+ b.getParameters().getRadiusCircleWander()*b.getVelocity() 
    	+ b.getParameters().getRadiusCircleWander()*randomVec3;

    return arrive(b, desiredTarget);
}

// TODO : Maybe reuse seek
glm::vec3 MovableState::arrive(const MovableBoid& b, const glm::vec3 & target) const
{

	glm::vec3 desiredVelocity = target - b.getLocation();

	float distance = glm::length(desiredVelocity);
	glm::normalize(desiredVelocity);
	if (distance < b.getParameters().getDistStartSlowingDown()) {
		// Set the magnitude according to how close we are.
		// Linear computation of the magnitude
	  	float magnitude = distance * b.getParameters().getMaxSpeed()
	  		/ b.getParameters().getDistStartSlowingDown();
	  desiredVelocity *= magnitude;
	} else {
	  // Otherwise, proceed at maximum speed.
	  desiredVelocity *= b.getParameters().getMaxSpeed();
	}

	// The usual steering = desiredVelocity - velocity
	glm::vec3 steer = desiredVelocity - b.getVelocity();
	limitVec3(steer, b.getParameters().getMaxForce());
	return steer;
}

glm::vec3 MovableState::stayWithinWalls(const MovableBoid& b) const
{
	glm::vec3 steer(0, 0, 0);
	float distToWall = 20.0f;
    if (b.getLocation().x < -distToWall) {
    	glm::vec3 desired(b.getParameters().getMaxForce(), b.getVelocity().y, 0);
    	steer = desired - b.getVelocity();
    } else if (b.getLocation().x >  distToWall) {
		glm::vec3 desired(-b.getParameters().getMaxForce(), b.getVelocity().y, 0);
		steer = desired - b.getVelocity() ;
    }
    
    if (b.getLocation().y < -distToWall) {
		glm::vec3 desired(b.getVelocity().x, b.getParameters().getMaxForce(), 0);
		steer += desired - b.getVelocity();
    } else if (b.getLocation().y >  distToWall) {
		glm::vec3 desired(b.getVelocity().x, -b.getParameters().getMaxForce(), 0);
		steer += desired - b.getVelocity();
    }

    steer = limitVec3(steer, b.getParameters().getMaxForce());

    return steer;
}

glm::vec3 MovableState::separate(const MovableBoid& b, const std::vector<MovableBoidPtr> & bVec) const
{
	glm::vec3 sum(0,0,0);
	glm::vec3 steer(0,0,0);
	int count = 0;
	for(BoidPtr bElt : bVec) {
		float d = glm::distance(b.getLocation(), bElt->getLocation());
		if ((d > 0) && b.distVision(*bElt, b.getParameters().getDistSeparate())) {
			glm::vec3 diff = b.getLocation() - bElt->getLocation();
			diff = glm::normalize(diff);
			sum += diff;
			count++;
		}
	}
	if (count > 0) {
		sum /= count;
		sum = glm::normalize(sum) * b.getParameters().getMaxSpeed();
		steer = sum - b.getVelocity();
		steer = limitVec3(steer, b.getParameters().getMaxForce());
	}
	return steer;
}

glm::vec3 MovableState::collisionAvoid (const MovableBoid& b, const std::vector<RootedBoidPtr> & rootB) const
{
	float coeff = glm::length(b.getVelocity()) / b.getParameters().getMaxSpeed();
	glm::vec3 posAdhead = b.getLocation() + coeff * cNormalize(b.getVelocity()) * b.getParameters().getDistSeeAhead();
	glm::vec3 posAdhead2 = b.getLocation() + coeff *  cNormalize(b.getVelocity()) * b.getParameters().getDistSeeAhead() * 0.5f;
	std::vector<RootedBoidPtr>::const_iterator it = rootB.begin();
	RootedBoidPtr eltFar = (RootedBoidPtr) nullptr;
	RootedBoidPtr eltClose = (RootedBoidPtr) nullptr;
	RootedBoidPtr elt;
	while (it != rootB.end() && eltClose == (RootedBoidPtr) nullptr) {
		elt = *it;
		if (glm::distance(elt->getLocation(), posAdhead) <= elt->getRadius()) {
			eltFar = elt;
		} else if (glm::distance(elt->getLocation(), posAdhead2) <= elt->getRadius()) {
			eltClose = elt;
		}
		it++;
	}
	if (!(eltClose == (RootedBoidPtr) nullptr)) {
		return glm::normalize(posAdhead - eltClose->getLocation()) * b.getParameters().getMaxForce();
	} else if (!(eltFar == (RootedBoidPtr) nullptr)) {
		return glm::normalize(posAdhead - eltFar->getLocation()) * b.getParameters().getMaxForce();
	} else {
		return glm::vec3(0,0,0);
	}
}

glm::vec3 MovableState::align (const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB) const
{
	glm::vec3 sum(0,0,0);
	glm::vec3 steer;
	int count = 0;
	for (MovableBoidPtr other : mvB) {
		if(b.sameSpecies(*other)) 
		{
			float d = glm::distance(b.getLocation(), other->getLocation());
			if ((d > 0) && (d < b.getParameters().getDistViewCohesion()) && b.canSee(*other, b.getParameters().getAngleView())) 
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
		sum = cNormalize(sum);
		sum *= b.getParameters().getMaxForce();
		steer = sum - b.getVelocity();
		steer = limitVec3(steer, b.getParameters().getMaxForce());
	} else {
		steer = glm::vec3(0,0,0);
	}

	return steer;
}

glm::vec3 MovableState::cohesion (const MovableBoid & b, const std::vector<MovableBoidPtr> & mvB) const
{
    glm::vec3 sum(0,0,0);
    glm::vec3 steer;
    int count = 0;
    for (MovableBoidPtr other : mvB) {
    	if(b.sameSpecies(*other)) 
		{
			float d = glm::distance(b.getLocation(), other->getLocation());
			if ((d > 0) && (d < b.getParameters().getDistViewCohesion()) && b.canSee(*other, b.getParameters().getAngleView())) {
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
									/ (hunter.getParameters().getMaxSpeed());
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
									/ (hunter.getParameters().getMaxSpeed());
	return flee(prey, positionForecast(hunter, dt, val));
}

// Precondition b.hasLeader() == true
glm::vec3 MovableState::followLeader(const MovableBoid & b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	if (!b.hasLeader()) {
		throw std::invalid_argument("The requested boid can't follow a leader when it has none");
	}
	glm::vec3 steer(0,0,0);
	MovableBoidPtr leader = b.getLeader();
	if (leader->getVelocity() == glm::vec3(0,0,0)) { // Trick to avoid error with normalize when leader->getVelocity() == (0,0,0)
		return glm::vec3(0, 0, 0); // Don't compute any force, it is fine
	}
	glm::vec3 positionBehindLeader = leader->getLocation() + glm::normalize(-1.0f * leader->getVelocity()) * b.getParameters().getDistToLeader();
	steer = arrive(b, positionBehindLeader);
	steer += 20.0f * separate(b, mvB); // Coefficient can be modify
	if (leader->canSee(b, 1.4f * leader->getParameters().getDistSeparate())) { // Can be modify
		steer += 20.0f * evade(b, *leader, dt);
	}
	limitVec3(steer, b.getParameters().getMaxForce());
	return steer;	
}

void MovableState::detectDanger(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB) const
{
	BoidType predator = b.getPredator();
	bool predatorFound = false;

	std::vector<MovableBoidPtr>::const_iterator it = mvB.begin();
	while(!predatorFound && it != mvB.end()) {
		if((*it)->getBoidType() == predator && b.canSee(**it, b.getParameters().getDistViewMax())) {
			predatorFound = true;
		}
		++it;
	}

	if(predatorFound) {
		b.getParameters().dangerIncrease();
	} else {
		b.getParameters().dangerDecrease();
	}
}

bool MovableState::alone(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB) const
{
	bool friendFound = false;

	std::vector<MovableBoidPtr>::const_iterator it = mvB.begin();
	while(!friendFound && it != mvB.end()) {
		if(b != **it && (*it)->getBoidType() == b.getBoidType() && b.canSee(**it, b.getParameters().getDistViewMax())) {
			friendFound = true;
		}
		++it;
	}

	if(friendFound) {
		b.getParameters().affinityIncrease();
	} else {
		b.getParameters().affinityDecrease();
	}
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
glm::vec3 TestState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	/*
	if(b.getBoidType() == WOLF) {
		return pursuit(b, *(b.getPrey()), dt) + 10.0f * stayWithinWalls(b);
	} else {
		return evade(b, *(b.getHunter()), dt) + 0.5f * wander(b) + 10.0f * stayWithinWalls(b);
	}
	*/
	std::vector<MovableBoidPtr> mvB = boidsManager.getMovableBoids();

	b.getParameters().staminaDecrease();
	b.getParameters().hungerDecrease();
	b.getParameters().thirstDecrease();

	// Detect danger and update danger parameter 
	detectDanger(b, mvB);

	// Detect if alone and update affinity
	alone(b, mvB);

	glm::vec3 newForces = wander(b) + 60.0f * stayWithinWalls(b) + 15.0f * collisionAvoid(b, boidsManager.getRootedBoids());
	return newForces;
}

glm::vec3 WalkState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	std::vector<MovableBoidPtr> mvB = boidsManager.getMovableBoids();

	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	b.getParameters().staminaDecrease();
	b.getParameters().hungerDecrease();
	b.getParameters().thirstDecrease();

	// Detect danger and update danger parameter 
	detectDanger(b, mvB);

	// Detect if alone and update affinity
	alone(b, mvB);

	glm::vec3 newForces(0,0,0);
	if(b.hasLeader() && b.canSee(*b.getLeader(), b.getParameters().getDistViewMax())) { // Can see the leader
		newForces = 40.0f * followLeader(b, mvB, dt);
	}
	else if (b.isLeader()) {
		newForces = wander(b);
	} else { // Can't see the leader
		newForces = wander(b) + 20.0f * separate(b, mvB) + 1.0f * cohesion(b, mvB) + 3.0f * align(b, mvB); 
	}
	newForces += 60.0f * stayWithinWalls(b);
	
	newForces.z = 0.0f; // Trick to stay in 2D change with the height map when in 3D
	return newForces;
}

glm::vec3 StayState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// stamina <- si(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)

	std::vector<MovableBoidPtr> mvB = boidsManager.getMovableBoids();

	b.getParameters().staminaIncrease();
	b.getParameters().hungerDecrease();
	b.getParameters().thirstDecrease();

	// Detect danger and update danger parameter 
	detectDanger(b, mvB);

	// Detect if alone and update affinity
	alone(b, mvB);

	return arrive(b, b.getLocation());
}

glm::vec3 SleepState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// /!\ requirement : danger <= lowDanger
	// stamina <- si(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// danger did'nt change the boid is not aware of his surronding
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)

	std::vector<MovableBoidPtr> mvB = boidsManager.getMovableBoids();

	b.getParameters().staminaIncrease();
	b.getParameters().hungerDecrease();
	b.getParameters().thirstDecrease();

	// Detect if alone and update affinity
	alone(b, mvB);

	return arrive(b, b.getLocation());
}

glm::vec3 FleeState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// TODO : velocity <- max_speed in the opposite direction of predators
	// stamina <- min(sd(stamina) * cste, 0)
	// hunger <- min(hd(hunger) * cste, 0)
	// thirst <- min(td(thirst) * cste, 0)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity) 
	return glm::vec3(0,0,0);
}

MovableBoidPtr closestAnimal(const MovableBoid & b, const BoidType & type, const std::vector<MovableBoidPtr> & mvB) {
	// TODO : After optimization, call getNeighboor(b)
	float tmpDistance = FLT_MAX;
	MovableBoidPtr target = (MovableBoidPtr) nullptr;

	for (MovableBoidPtr m : mvB) {
		if (b.canSee(*m, b.getParameters().getDistViewMax()) && b.distVision(*m, tmpDistance) && m->getBoidType() == type) { 
			// TODO : optimization possible		
			tmpDistance = glm::distance(m->getLocation(), b.getLocation());
			target = m;
		}
	}
	return target;
}

glm::vec3 FindFoodState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// TODO : wander or follow group until the boid find sth (we can mix both in funtion of hunger variable)
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	
	const std::vector<MovableBoidPtr> mvB = boidsManager.getMovableBoids();

	MovableBoidPtr target;
	switch (b.getBoidType()) {
		case WOLF:
			target = closestAnimal(b, RABBIT, mvB);
			if (target == (MovableBoidPtr) nullptr) {
				return wander(b) + 16.0f * stayWithinWalls(b);
			} else {
				// TODO : Chase
				return arrive(b, target->getLocation());
			}
			break;
		case RABBIT:
			// Can't find the target because it is not in mvB list
			target = closestAnimal(b, CARROT, mvB);
			if (target == (MovableBoidPtr) nullptr) {
				return wander(b) + 16.0f * stayWithinWalls(b);
			} else {
				// TODO : Chase
				return arrive(b, target->getLocation());
			}
			break;
		default:
			std::cerr << "Unknown animal looking for food" << std::endl;
			return glm::vec3(0,0,0);
			break;
	}
	return glm::vec3(0,0,0);
}

glm::vec3 EatState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// stamina <- si(stamina)
	// hunger <- hi(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	std::vector<MovableBoidPtr> mvB = boidsManager.getMovableBoids();

	b.getParameters().staminaIncrease();
	b.getParameters().hungerIncrease();
	b.getParameters().thirstDecrease();

	// Detect danger and update danger parameter 
	detectDanger(b, mvB);

	// Detect if alone and update affinity
	alone(b, mvB);

	///< TODO maybe it better with the target ?
	return arrive(b, b.getLocation());
}

glm::vec3 FindWaterState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// TODO : wander or follow group until the boid find sth (we can mix both in funtion of thirst variable)
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	return glm::vec3(0,0,0);
}

glm::vec3 DrinkState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// TODO : velocity <- (0,0,0)
	// stamina <- si(stamina)
	// hunger <- hd(hunger)
	// thirst <- ti(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	std::vector<MovableBoidPtr> mvB = boidsManager.getMovableBoids();
	
	b.getParameters().staminaIncrease();
	b.getParameters().hungerDecrease();
	b.getParameters().thirstIncrease();

	// Detect danger and update danger parameter 
	detectDanger(b, mvB);

	// Detect if alone and update affinity
	alone(b, mvB);

	///< TODO maybe it better with the target ?
	return arrive(b, b.getLocation());
}

glm::vec3 MateState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// TODO : velocity <- (0,0,0) && create a new boid
	// requirement : danger <= lowDanger
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	return glm::vec3(0,0,0);
}

glm::vec3 AttackState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// TODO : velocity <- max velocity to taget
	// stamina <- decrease
	// hunger <- decrease
	// thirst <- decrease
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if alone affinity <- ad(affinity)
	return glm::vec3(0,0,0);
}

glm::vec3 LostState::computeNewForces(const MovableBoid& b, const BoidsManager & boidsManager, const float & dt) const
{
	// TODO Think about it
	return glm::vec3(0,0,0);
}
