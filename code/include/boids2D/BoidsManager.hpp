#ifndef BOIDS_MANAGER_HPP
#define BOIDS_MANAGER_HPP

#include <vector>

#include "MovableBoid.hpp"
#include "RootedBoid.hpp"
#include "BoidType.hpp"

class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class BoidsManager
{
 public:
	BoidsManager(void);
	
	const std::vector<MovableBoidPtr>& getMovableBoids() const;

	const std::vector<RootedBoidPtr>& getRootedBoids() const;

	RootedBoidPtr addRootedBoid(BoidType boidType, glm::vec3 location);

 	MovableBoidPtr addMovableBoid(BoidType boidType, glm::vec3 location, glm::vec3 velocity = glm::vec3(0,0,0));

	bool isNight() const;

	void setTimeDay(bool state);

 private:
 	std::vector<MovableBoidPtr> m_movableBoids;
 	std::vector<RootedBoidPtr> m_rootedBoids;
 	bool isNightTime;
};

typedef std::shared_ptr<BoidsManager> BoidsManagerPtr;

#endif