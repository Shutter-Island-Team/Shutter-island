#ifndef MOVABLE_BOID_HPP
#define MOVABLE_BOID_HPP

#include "Boid.hpp"
#include <vector>

class MovableBoid : public Boid
{
 public:
	MovableBoid(glm::vec3 position, TypeBoid t);

	void computeNextStep();

	glm::vec3 getVelocity();

	void move (std::vector<MovableBoid>& movableBoids);

 private:
  glm::vec3 m_velocity;
  glm::vec3 m_acceleration;
  glm::vec3 m_mass;

  const float MAX_SPEED = 2; 
  const float NORMAL_SPEED = 1.5; 
  const float MAX_FORCE = 2; 

  float m_maxSpeed = MAX_SPEED;
  float m_normalSpeed = NORMAL_SPEED;
  float m_maxForce = MAX_FORCE;

  bool isNeighbor(Boid b);
  bool isNear(Boid b);
  glm::vec3 ruleFlyToCenter(std::vector<MovableBoid>& movableBoids);
  glm::vec3 ruleKeepDistance(std::vector<MovableBoid>& movableBoids);
  glm::vec3 ruleMatchSpeed(std::vector<MovableBoid>& movableBoids);
  void applyForce(glm::vec3 f);
  void update();

  
};

#endif