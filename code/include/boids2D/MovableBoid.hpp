#ifndef MOVABLE_BOID_HPP
#define MOVABLE_BOID_HPP

#include "Boid.hpp"
#include <vector>
#include <cmath>

#include "MovableState.hpp"
#include "MovableParameters.hpp"

class MovableState;
class MovableParameters;

class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class MovableBoid : public Boid
{
 public:
	MovableBoid(glm::vec3 location, BoidType t, MovableParameters* parameters);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, BoidType t, MovableParameters* parameters);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass, BoidType t, MovableParameters* parameters);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, BoidType t, MovableParameters* parameters);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, float maxSpeed, 
    float maxForce, BoidType t, MovableParameters* parameters);

  void initializeParameters(MovableBoidPtr thisBoid);

	bool canSee(Boid b, float distView);

	void computeNextStep(float dt);

  bool angleVision (Boid b);

	glm::vec3 getVelocity();

  float getMass();

  glm::vec3 wander();

  glm::vec3 ruleStayWithinWalls();

  glm::vec3 arrive(glm::vec3 target);

  glm::vec3 separate(std::vector<MovableBoidPtr> mvB, float desiredSeparation);

  glm::vec3 computeAcceleration(std::vector<MovableBoidPtr> mvB);

  glm::vec3 getAcceleration();

  void setAcceleration(glm::vec3 acceleration);

 protected:
  glm::vec3 m_velocity;
  glm::vec3 m_acceleration;
  float m_mass;
  float m_angleView;
  float m_distViewSeparate;
  float m_distViewCohesion;
  float m_maxSpeed;
  float m_maxForce;

  // Variable to wander
  float rCircleWander = 9.0f;
  float distToCircle = 3.0f;
  float distStartSlowingDown = 5.0f;

  MovableState* m_currentState;
  MovableParameters* m_parameters;

 private:
  glm::vec3 separate(std::vector<MovableBoidPtr> mvB);

  glm::vec3 align (std::vector<MovableBoidPtr> mvB);

  glm::vec3 cohesion (std::vector<MovableBoidPtr> mvB);
};

bool operator==(const MovableBoid& b1, const MovableBoid& b2);

bool operator!=(const MovableBoid& b1, const MovableBoid& b2);


#endif