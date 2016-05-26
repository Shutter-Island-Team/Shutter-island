#ifndef BOIDS_MANAGER_HPP
#define BOIDS_MANAGER_HPP

#include <vector>

#include "MovableBoid.hpp"
#include "RootedBoid.hpp"
#include "BoidType.hpp"
#include "Wolf.hpp"
#include "Rabbit.hpp"
#include "Carrot.hpp"
#include "Tree.hpp"

class BoidsManager
{
 public:
	BoidsManager(void);
	
	std::vector<MovableBoidPtr>& getMovableBoids();

	std::vector<RootedBoidPtr>& getRootedBoids();

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