#ifndef BOIDS_MANAGER_HPP
#define BOIDS_MANAGER_HPP

#include <vector>

#include "Boid.hpp"


class BoidsManager
{
 public:
	BoidsManager(void);

 private:
 	std::vector<Boid> boids;
};

#endif