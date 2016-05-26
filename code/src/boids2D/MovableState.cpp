#include "../../include/boids2D/MovableState.hpp"

#include "../../include/Utils.hpp"
#include <iostream>

glm::vec3 MovableState::computeAcceleration(MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	// Reset acceleration
	b.resetAcceleration();
	return computeNewForces(b, mvB, dt);
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

glm::vec3 MovableState::separate(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB) const
{
	glm::vec3 sum(0,0,0);
	glm::vec3 steer(0,0,0);
	int count = 0;
	for(MovableBoidPtr m : mvB) {
		float d = glm::distance(b.getLocation(), m->getLocation());
		if ((d > 0) && b.distVision(*m, b.getParameters().getDistSeparate())) {
			glm::vec3 diff = b.getLocation() - m->getLocation();
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
		sum = glm::normalize(sum);
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
		steer = seek(b, sum);
    } else {
    	steer = glm::vec3(0,0,0);
    }

    return steer;
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
	MovableBoidPtr leader = b.getParameters().getLeader();
	if (leader->getVelocity() == glm::vec3(0,0,0)) { // Trick to avoid error with normalize when leader->getVelocity() == (0,0,0)
		return glm::vec3(0, 0, 0); // Don't compute any force, it is fine
	}
	glm::vec3 positionBehindLeader = leader->getLocation() + glm::normalize(-1.0f * leader->getVelocity()) * b.getParameters().getDistToLeader();
	steer = arrive(b, positionBehindLeader);
	steer += 3.0f * separate(b, mvB); // Coefficient can be modify
	if (leader->canSee(b, 3.0f * leader->getParameters().getDistSeparate())) { // Can be modify
		steer += evade(b, *leader, dt);
	}
	limitVec3(steer, b.getParameters().getMaxForce());
	return steer;	
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
glm::vec3 TestState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	if(b.getBoidType() == WOLF) {
		return pursuit(b, *(b.getPrey()), dt) + 10.0f * stayWithinWalls(b);
	} else {
		return evade(b, *(b.getHunter()), dt) + 0.5f * wander(b) + 10.0f * stayWithinWalls(b);
	}
}

glm::vec3 WalkState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	// TODO : compute new forces (depend on the species ex : wolf follow leader and leader wonder)
	// use BoidType to determine species
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	/*
	glm::vec3 newForces = 1.0f * wander(b) + 4.0f * separate(b, mvB) + 4.0f * cohesion(b, mvB) + 4.0f * align(b, mvB) + 16.0f * stayWithinWalls(b);
	switch(b.getBoidType()) 
	{
		case RABBIT:
		break;
		case WOLF:
			if(b.canSee(*b.getParameters().getLeader(), b.getParameters().getDistViewMax()))
			{
				// follow leader
				newForces += 8.0f * arrive(b, b.getParameters().getLeader()->getLocation());
			} 
			else
			{
				// wander
				// nothing to do
			}
			
		break;
	}
	*/
/*
	std::cout << "Leader : (" << b.getParameters().getLeader()->getLocation().x 
		<< ", " << b.getParameters().getLeader()->getLocation().y
		<< ", " << b.getParameters().getLeader()->getLocation().z << ")" << std::endl;
*/

	b.getParameters().staminaDecrease(1.01f);
	//b.getParameters().hungerDecrease();
	glm::vec3 newForces = 1.0f * wander(b) + 4.0f * separate(b, mvB) + 4.0f * cohesion(b, mvB) 
		+ 4.0f * align(b, mvB) + 160.0f * stayWithinWalls(b);
	newForces.z = 0.0f;
	return newForces;
}

glm::vec3 StayState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	// TODO : velocity <- (0,0,0)
	// stamina <- si(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	b.getParameters().staminaIncrease(0.5f);
	return arrive(b, b.getLocation());
}

glm::vec3 SleepState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	// TODO : velocity <- (0,0,0)
	// requirement : danger <= lowDanger
	// stamina <- max(si(stamina) * cste, 100)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// danger <- danger
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	return glm::vec3(0,0,0);
}

glm::vec3 FleeState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
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

glm::vec3 FindFoodState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	// TODO : wander or follow group until the boid find sth (we can mix both in funtion of hunger variable)
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	
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

glm::vec3 EatState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	// TODO : velocity <- (0,0,0)
	// stamina <- si(stamina)
	// hunger <- hi(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	return glm::vec3(0,0,0);
}

glm::vec3 FindWaterState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
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

glm::vec3 DrinkState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	// TODO : velocity <- (0,0,0)
	// stamina <- si(stamina)
	// hunger <- hd(hunger)
	// thirst <- ti(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	return glm::vec3(0,0,0);
}

glm::vec3 MateState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
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

glm::vec3 AttackState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	// TODO : velocity <- max velocity to taget
	// stamina <- decrease
	// hunger <- decrease
	// thirst <- decrease
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if alone affinity <- ad(affinity)
	return glm::vec3(0,0,0);
}

glm::vec3 LostState::computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const
{
	// TODO Think about it
	return glm::vec3(0,0,0);
}
