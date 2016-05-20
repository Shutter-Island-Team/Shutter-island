#ifndef WALK_STATE_HPP
#define WALK_STATE_HPP

#include "MovableState.hpp"
#include <glm/glm.hpp>

class WalkState : public MovableState 
{

private:
	glm::vec3 computeNewForces();
};

#endif