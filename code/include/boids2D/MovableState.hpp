#ifndef MOVABLE_STATE_HPP
#define MOVABLE_STATE_HPP

#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include "MovableBoid.hpp"

class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class MovableState 
{
public:

	glm::vec3 computeAcceleration(MovableBoid& b, std::vector<MovableBoidPtr> mvB);

private:
	virtual glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB) = 0;
};

class TestState : public MovableState 
{

private:
	float t_maxSpeed = 3.5f;
  	float t_maxForce = 2.0f;

	float t_rCircleWander = 9.0f;
  	float t_distToCircle = 3.0f;
  	float t_distStartSlowingDown = 5.0f;

	glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB);
	glm::vec3 wander(MovableBoid& b);
	glm::vec3 arrive(MovableBoid& b, glm::vec3 target);
};

class WalkState : public MovableState 
{

private:
	glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB);
};

class StayState : public MovableState 
{

private:
	glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB);
};

class SleepState : public MovableState 
{

private:
	glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB);
};

class FleeState : public MovableState 
{

private:
	glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB);
};

class FindFoodState : public MovableState 
{

private:
	glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB);
};

class EatState : public MovableState 
{

private:
	glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB);
};

class FindWaterState : public MovableState 
{

private:
	glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB);
};

class DrinkState : public MovableState 
{

private:
	glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB);
};

class MateState : public MovableState 
{

private:
	glm::vec3 computeNewForces(MovableBoid& b, std::vector<MovableBoidPtr> mvB);
};


#endif