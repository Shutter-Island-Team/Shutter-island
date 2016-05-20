#ifndef MOVABLE_STATE_HPP
#define MOVABLE_STATE_HPP

#include <glm/glm.hpp>

class MovableState 
{
public:

	glm::vec3 computeAcceleration();

private:
	virtual glm::vec3 computeNewForces() = 0;
};

#endif