#ifndef ROOTED_BOID_HPP
#define ROOTED_BOID_HPP

#include "Boid.hpp"

class RootedBoid : public Boid
{
public:
	RootedBoid(glm::vec3 location, BoidType t);
	
protected:
};

#endif