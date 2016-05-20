#ifndef BOID_HPP
#define BOID_HPP

#include <glm/glm.hpp>
#include <memory>

#include <vector>
#include "BoidType.hpp"

class Boid
{
 public:
	Boid(glm::vec3 location, BoidType t);

	glm::vec3 getLocation();

	void setLocation(glm::vec3 location);

	bool isMovable();

	float getAngle();

	void setAngle(float angle);

	void setTarget(float x, float y);

	glm::vec2 getTarget();

	float getMinDistance();

	void setMinDistance(float minDistance);

	BoidType getBoidType();

 protected:
 	glm::vec3 m_location;
 	float m_angle;
 	BoidType m_boidType;
 	bool m_movable;
	glm::vec2 m_target;

 	float m_minDistance = 1;
};

typedef std::shared_ptr<Boid> BoidPtr;

#endif
