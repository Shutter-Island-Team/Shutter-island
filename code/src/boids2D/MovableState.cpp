#include "../../include/boids2D/MovableState.hpp"

#include "../../include/Utils.hpp"
#include <iostream>

glm::vec3 MovableState::computeAcceleration(MovableBoid& b, std::vector<MovableBoidPtr> mvB) 
{
	// Reset acceleration
	b.resetAcceleration();
	return computeNewForces(b, mvB);
}

glm::vec3 MovableState::wander(MovableBoid& b)
{
	float randomVal = random(0.0f, 2*M_PI);
	glm::vec3 randomVec3(cos(randomVal), sin(randomVal), 0);
    glm::vec3 desiredTarget = b.getLocation() + b.getParameters().getRadiusCircleWander()*b.getVelocity() + b.getParameters().getRadiusCircleWander()*randomVec3;
    return arrive(b, desiredTarget);
}

glm::vec3 MovableState::arrive(MovableBoid& b, glm::vec3 target)
{
	glm::vec3 desired = target - b.getLocation();

	float d = glm::length(desired);
	glm::normalize(desired);
	if (d < b.getParameters().getDistStartSlowingDown()) {
	  // Set the magnitude according to how close we are.
	  float m = d*b.getParameters().getMaxSpeed()/b.getParameters().getDistStartSlowingDown();
	  desired *= m;
	} else {
	  // Otherwise, proceed at maximum speed.
	  desired *= b.getParameters().getMaxSpeed();
	}

	// The usual steering = desired - velocity
	glm::vec3 steer = desired - b.getVelocity();
	if (glm::length(steer) > b.getParameters().getMaxForce()) {
		steer = glm::normalize(steer)*b.getParameters().getMaxForce();
	}

	return steer;
}

glm::vec3 MovableState::stayWithinWalls(MovableBoid& b) {
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

glm::vec3 MovableState::separate(MovableBoid& b, std::vector<MovableBoidPtr> mvB){
	glm::vec3 sum(0,0,0);
	int count = 0;
	for(MovableBoidPtr m : mvB) {
		float d = glm::distance(b.getLocation(), m->getLocation());
		if ((d > 0) && b.distVision(*m, b.getParameters().getDistSeparate())) {
			glm::vec3 diff = b.getLocation() - m->getLocation();
			diff = glm::normalize(diff) / d;
			sum += diff;
			count++;
		}
	}
	if (count > 0) {
		sum /= count;
		sum = glm::normalize(sum) * b.getParameters().getMaxForce();
		glm::vec3 steer = sum - b.getVelocity();
		sum = limitVec3(steer, b.getParameters().getMaxForce());
	}
	return sum;
}

glm::vec3 MovableState::align (MovableBoid& b, std::vector<MovableBoidPtr> mvB) {
	glm::vec3 sum(0,0,0);
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
		// sum.normalize();
		// sum.mult(maxspeed);
		sum = glm::normalize(limitVec3(sum - b.getVelocity(), b.getParameters().getMaxForce()));
	}
	return sum;
}

glm::vec3 MovableState::cohesion (MovableBoid& b, std::vector<MovableBoidPtr> mvB) {
    glm::vec3 sum(0,0,0);
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
		// Here we make use of the seek() function we
		// wrote in Example 6.8.  The target
		// we seek is the average location of
		// our neighbors.
    }

    return sum;
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


glm::vec3 WalkState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
{
	// TODO : compute new forces (depend on the species ex : wolf follow leader and leader wonder)
	// use BoidType to determine species
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	glm::vec3 newForces = 1.0f * wander(b) + 4.0f * separate(b, mvB) + 4.0f * cohesion(b, mvB) + 4.0f * align(b, mvB) + 16.0f * stayWithinWalls(b);;
	switch(b.getBoidType()) 
	{
		case RABBIT:
		break;
		case WOLF:
			if(b.getParameters().getLeader() != nullptr)
			{
				newForces += 8.0f * arrive(b, b.getParameters().getLeader()->getLocation());
			}
			/*
			if(b.canSee(*b.getParameters().getLeader(), b.getParameters().getDistViewMax()))
			{
				// follow leader
				
			} 
			else
			{
				// wander
				// nothing to do
			}
			*/
		break;
	}

/*
	std::cout << "Leader : (" << b.getParameters().getLeader()->getLocation().x 
		<< ", " << b.getParameters().getLeader()->getLocation().y
		<< ", " << b.getParameters().getLeader()->getLocation().z << ")" << std::endl;
*/

	b.getParameters().staminaDecrease(0.5f);
	b.getParameters().hungerDecrease();

	return newForces;
}

glm::vec3 StayState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
{
	// TODO : velocity <- (0,0,0)
	// stamina <- si(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
	b.getParameters().staminaIncrease(3.0f);
	return arrive(b, b.getLocation());
}

glm::vec3 SleepState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
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

glm::vec3 FleeState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
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

glm::vec3 FindFoodState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
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

glm::vec3 EatState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
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

glm::vec3 FindWaterState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
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

glm::vec3 DrinkState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
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

glm::vec3 MateState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
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

glm::vec3 AttackState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
{
	// TODO : velocity <- max velocity to taget
	// stamina <- decrease
	// hunger <- decrease
	// thirst <- decrease
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if alone affinity <- ad(affinity)
	return glm::vec3(0,0,0);
}

glm::vec3 LostState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
{
	// TODO Think about it
	return glm::vec3(0,0,0);
}
