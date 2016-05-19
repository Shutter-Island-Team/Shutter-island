#ifndef BOIDS_MANAGER_HPP
#define BOIDS_MANAGER_HPP

#include <vector>

#include "MovableBoid.hpp"
#include "TypeBoid.hpp"


class BoidsManager
{
 public:
	BoidsManager(void);

	void add(MovableBoid b);
	void update();

 private:
 	void moveAllBoids();
 	std::vector<MovableBoid> m_movableBoids;
};

#endif