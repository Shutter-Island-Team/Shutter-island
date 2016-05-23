#ifndef CARROT_BOID_HPP
#define CARROT_BOID_HPP

#include "RootedBoid.hpp"
#include "BoidType.hpp"

class Carrot : public RootedBoid
{
	Carrot(glm::vec3 location);
};

#endif