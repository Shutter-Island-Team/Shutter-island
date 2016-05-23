#ifndef TREE_BOID_HPP
#define TREE_BOID_HPP

#include "RootedBoid.hpp"
#include "BoidType.hpp"

class Tree : public RootedBoid
{
	Tree(glm::vec3 location);
};

#endif