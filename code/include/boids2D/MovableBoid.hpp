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
	MovableBoid(glm::vec3 location, BoidType t, MovableParameters* parameters);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, BoidType t,
              MovableParameters* parameters);

  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass, BoidType t,
              MovableParameters* parameters);

  ~MovableBoid();

  /**
   * @brief     This function is used for a MovableBoidPtr to be
   *            linked with its parameter
   * @param[in] boidPtr The boid to get the parameter from
   */
  void associateBoid(MovableBoidPtr boidPtr);

  /**
   * @brief Getter for the velocity of the object
   */
  glm::vec3 getVelocity();

  /**
   * @brief Getter for the mass of the object
   */
  float getMass();

  /**
   * @brief   Getter for the parameter of the object
   * @return  Parameter of the object
   */
  MovableParameters & getParameters() const;

  /**
   * @brief Set the acceleration field of the class to 0
   */
  void resetAcceleration();

  /**
   * @brief     Update the acceleration of the boid. Save the value
   *            in the acceleration field of the class
   * @param[in] mvB Other movable boid the object is aware of
   */
  void computeAcceleration(std::vector<MovableBoidPtr> mvB);

  /**
   * @brief     Update the position and the velocity for the next step in the simulation 
   * @param[in] dt Value of the time step
   */
  void computeNextStep(float dt);

  /**
   * @brief     Check if the other boid is in the cone of vision of this
   * @param[in] other     The other boid to check if in range
   * @param[in] distView  The maximum distance viewable by this
   */
	bool canSee(Boid other, float distView) const;

  /**
   * @brief     Check if the other boid is in the circle of center of the
   *            location of this and of the radius of distView 
   * @param[in] other     The other boid to check if in range
   * @param[in] distView  The radius of the circle
   * @return    true if in the circle, false otherwise
   */
  bool distVision (Boid other, float distView) const;

  /**
   * @brief     Check if the other boid is the same type of this
   * @param[in] other The other boid
   * @return    true if the other boid is the same type of the, false otherwise
   */
  bool sameSpecies(Boid other);

  /**
   * @brief     Check if the other boid is in the angle of view of this (distance don't matter)
   * @param[in] other The other boid
   * @return    true if the other boid is in the angle of view of this, false otherwise
   * @warning   If the angle of view of the object is equal to PI, this function might not work
   */
  bool angleVision (Boid other) const;

 private:
  glm::vec3 m_velocity;
  glm::vec3 m_acceleration;
  float m_mass;

  MovableState* m_currentState;
  MovableParameters* m_parameters;

  void walkStateHandler();

  void stayStateHandler();

  void findFoodStateHandler();

  StateType m_stateType;
};

/**
 * @brief   Comparator between 2 boid.
 * @return  true if b1 and b2 has the same position
 */
bool operator==(const MovableBoid& b1, const MovableBoid& b2);

/**
 * @brief Comparator between 2 boid. @see ==
 */
bool operator!=(const MovableBoid& b1, const MovableBoid& b2);


#endif