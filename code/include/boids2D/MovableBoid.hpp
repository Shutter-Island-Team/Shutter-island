#ifndef MOVABLE_BOID_HPP
#define MOVABLE_BOID_HPP

#include "Boid.hpp"
#include <vector>

#include <cmath>

class MovableBoid : public Boid
{
 public:
	MovableBoid(glm::vec3 position, TypeBoid t);

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

 private:
  glm::vec3 m_velocity;
  glm::vec3 m_acceleration;
  float m_mass = 0.05f;

  const float MAX_SPEED = 2.0f; 
  const float NORMAL_SPEED = 1.5f; 
  const float MAX_FORCE = 1.0f;

  float m_maxSpeed = MAX_SPEED;
  float m_normalSpeed = NORMAL_SPEED;
  float m_maxForce = MAX_FORCE;

  float m_angleView = M_PI;
  float m_distView = M_PI;
  bool isNeighbor(Boid b);
  bool isNear(Boid b);
  glm::vec3 ruleFlyToCenter(std::vector<MovableBoid>& movableBoids);
  glm::vec3 ruleKeepDistance(std::vector<MovableBoid>& movableBoids);
  glm::vec3 ruleMatchSpeed(std::vector<MovableBoid>& movableBoids);

  glm::vec3 separate(std::vector<MovableBoid> mvB, float desiredSeparation);

  // Variable to wander
  float rCircleWander = 9.0f;
  float distToCircle = 3.0f;
  float distStartSlowingDown = 5.0f;

};

typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

#endif