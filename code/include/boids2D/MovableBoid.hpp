#ifndef MOVABLE_BOID_HPP
#define MOVABLE_BOID_HPP

#include "Boid.hpp"
#include <vector>

#include <cmath>

class MovableBoid : public Boid
{
 public:
	MovableBoid(glm::vec3 location, BoidType t);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, BoidType t);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass, BoidType t);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distView, BoidType t);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distView, float maxSpeed, 
    float maxForce, BoidType t);

	bool canSee(Boid b);

	void computeNextStep(float dt);

  bool angleVision (Boid b);

	glm::vec3 getVelocity();

  void setVelocity(glm::vec3 velocity);

  float getMass();

  glm::vec3 wander();

  glm::vec3 ruleStayWithinWalls();

  glm::vec3 arrive(glm::vec3 target);

  glm::vec3 computeAcceleration();

  glm::vec3 getAcceleration();

 protected:
  const float DEFAULT_MAX_SPEED = 10.0f; 
  const float DEFAULT_MAX_FORCE = 50.0f;
  const float DEFAULT_ANGLE_VIEW = M_PI;
  const float DEFAULT_DISTANCE_VIEW = M_PI;
  const float DEFAULT_MASS = 0.005f;


  glm::vec3 m_velocity;
  glm::vec3 m_acceleration;
  float m_mass;
  float m_angleView;
  float m_distView;
  float m_maxSpeed;
  float m_maxForce;

  
  // TODO : remove for here during the state machine développement
  // Variable to wander
  float rCircleWander = 3.0f;
  float distToCircle = 9.0f;
  float distStartSlowingDown = 5.0f;

};

typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

#endif