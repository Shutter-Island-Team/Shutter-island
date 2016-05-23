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
	b.getParameters().staminaDecrease();
	return 4.0f * wander(b);
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
	b.getParameters().staminaIncrease();
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

glm::vec3 FindFoodState::computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB)
{
	// TODO : wander or follow group until the boid find sth (we can mix both in funtion of hunger variable)
	// stamina <- sd(stamina)
	// hunger <- hd(hunger)
	// thirst <- td(thirst)
	// if predator is near danger <- di(danger) else danger <- dd(danger)
	// if in a group of same species affinity <- ai(affinity)
	// if alone affinity <- ad(affinity)
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