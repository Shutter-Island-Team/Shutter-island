#ifndef BOID_HPP
#define BOID_HPP

#include <glm/glm.hpp>

#include <vector>
#include "TypeBoid.hpp"

class Boid
{
 public:
	Boid(glm::vec3 position, TypeBoid t);

	glm::vec3 getLocation();

	void setLocation(glm::vec3 newLocation);

	bool isMovable();

	float getDistanceMin();

 private:
 	glm::vec3 m_location;
 	TypeBoid m_typeBoid;
 	bool m_movable;
 	float m_distanceMin = 1;
};

#endif