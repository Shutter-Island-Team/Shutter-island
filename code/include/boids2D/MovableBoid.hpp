#ifndef MOVABLE_BOID_HPP
#define MOVABLE_BOID_HPP

#include "Boid.hpp"
#include "StateType.hpp"
#include <vector>
#include <cmath>

#include "MovableState.hpp"
#include "MovableParameters.hpp"

class MovableState;
class TestState;
class MovableParameters;

class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class MovableBoid : public Boid
{
 public:
  ~MovableBoid();

	MovableBoid(glm::vec3 location, BoidType t, MovableParameters* parameters);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, BoidType t, MovableParameters* parameters);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass, BoidType t, MovableParameters* parameters);

  void initializeParameters(MovableBoidPtr thisBoid);

  glm::vec3 getVelocity();

  glm::vec3 getAcceleration();

  float getMass();

  MovableParameters & getParameters() const;

  void resetAcceleration();

  void computeAcceleration(std::vector<MovableBoidPtr> mvB);

  // Used in solver
  void computeNextStep(float dt);

	bool canSee(Boid b, float distView) const;

  bool distVision (Boid b, float distView) const;

  bool sameSpecies(Boid b);

  // Return the boolean if b is in the angle of vision of this
  // Warning : don't work if angleVision = PI
  bool angleVision (Boid b) const;

 private:
  glm::vec3 m_velocity;
  glm::vec3 m_acceleration;
  float m_mass;

  MovableState* m_currentState;
  MovableParameters* m_parameters;

  void walkStateHandler(std::vector<MovableBoidPtr> mvB);

  void stayStateHandler(std::vector<MovableBoidPtr> mvB);

  void findFoodStateHandler(std::vector<MovableBoidPtr> mvB);

  void setAcceleration(glm::vec3 acceleration);

  StateType m_stateType;
};

bool operator==(const MovableBoid& b1, const MovableBoid& b2);

bool operator!=(const MovableBoid& b1, const MovableBoid& b2);


#endif