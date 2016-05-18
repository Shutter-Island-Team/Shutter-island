#ifndef MOVABLE_BOID_HPP
#define MOVABLE_BOID_HPP

#include "Boid.hpp"

class MovableBoid : public Boid
{
 public:
	MovableBoid(glm::vec3 position, TypeBoid t);

	void computeNextStep();

 private:
  glm::vec3 m_velocity;
  glm::vec3 m_acceleration;

  const float MAX_SPEED = 2; 
  const float MAX_FORCE = 2; 

  float m_maxSpeed = MAX_SPEED;
  float m_maxForce = MAX_FORCE;
};

#endif