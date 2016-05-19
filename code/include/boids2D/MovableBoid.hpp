#ifndef MOVABLE_BOID_HPP
#define MOVABLE_BOID_HPP

#include "Boid.hpp"

#include <cmath>

class MovableBoid : public Boid
{
 public:
	MovableBoid(glm::vec3 position, TypeBoid t);

	bool canSee(Boid b);

	void computeNextStep();

  bool angleVision (Boid b);

 private:
  glm::vec3 m_velocity;
  glm::vec3 m_acceleration;

  const float MAX_SPEED = 2; 
  const float MAX_FORCE = 2; 

  float m_maxSpeed = MAX_SPEED;
  float m_maxForce = MAX_FORCE;

  float m_angleView = M_PI;
  float m_distView = M_PI;
};

#endif