#ifndef BOIDS_MANAGER_HPP
#define BOIDS_MANAGER_HPP

#include <vector>

#include "MovableBoid.hpp"
#include "TypeBoid.hpp"


class BoidsManager
{
 public:
	BoidsManager(void);

	void addMovable(MovableBoidPtr b);

	std::vector<MovableBoidPtr> & getMovableBoids();

 private:
 	std::vector<MovableBoidPtr> m_movableBoids;
};

#endif