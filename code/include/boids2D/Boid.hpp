#ifndef BOID_HPP
#define BOID_HPP

#include <glm/glm.hpp>

#include "TypeBoid.hpp"

class Boid
{
 public:
	Boid(glm::vec3 position, TypeBoid t);

	glm::vec3 getLocation();

	void setLocation(glm::vec3 newLocation);

 private:
 	glm::vec3 m_location;
 	TypeBoid m_typeBoid;
};

#endif