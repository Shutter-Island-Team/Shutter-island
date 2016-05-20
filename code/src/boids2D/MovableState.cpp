#include "../../include/boids2D/MovableState.hpp"

glm::vec3 MovableState::computeAcceleration() 
{
	return computeNewForces();
}