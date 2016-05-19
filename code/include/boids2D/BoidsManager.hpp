#ifndef BOIDS_MANAGER_HPP
#define BOIDS_MANAGER_HPP

#include <vector>

#include "MovableBoid.hpp"
#include "TypeBoid.hpp"


class BoidsManager
{
 public:
	BoidsManager(void);

	void addMovableBoid(MovableBoidPtr b);

	std::vector<MovableBoidPtr> & getMovableBoids();

 private:
 	std::vector<MovableBoidPtr> m_movableBoids;
};

typedef std::shared_ptr<BoidsManager> BoidsManagerPtr;

#endif