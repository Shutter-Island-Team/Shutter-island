#ifndef BOIDS_MANAGER_HPP
#define BOIDS_MANAGER_HPP

#include <vector>

#include "MovableBoid.hpp"
#include "RootedBoid.hpp"
#include "BoidType.hpp"

class BoidsManager
{
 public:
	BoidsManager(void);

	void addMovableBoid(MovableBoidPtr b);
	void addRootedBoid(RootedBoidPtr r);

	std::vector<MovableBoidPtr>& getMovableBoids();

	std::vector<RootedBoidPtr>& getRootedBoids();

	bool isNight() const;

	void setTimeDay(bool state);

 private:
 	std::vector<MovableBoidPtr> m_movableBoids;
 	std::vector<RootedBoidPtr> m_rootedBoids;
 	bool isNightTime;
};

typedef std::shared_ptr<BoidsManager> BoidsManagerPtr;

#endif