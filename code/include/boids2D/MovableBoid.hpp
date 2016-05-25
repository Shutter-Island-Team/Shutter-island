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

/**
 * @class MovableBoid
 * @brief Describe a movable boid
 * A MovaBoid has a velocity, an acceleration and is treated as
 * a boid which has behavior
 */
class MovableBoid : public Boid
{
 public:
  /**
   * @brief Constructor of a MovableBoid
   * @param[in] location    Initial location of the boid
   * @param[in] t           Type of the boid
   * @param[in] parameters  Parameter of the boid
   */
	MovableBoid(glm::vec3 location, BoidType t, MovableParameters* parameters);

  /**
   * @brief Constructor of a MovableBoid
   * @param[in] location    Initial location of the boid
   * @param[in] velocity    Initial velocity of the boid
   * @param[in] t           Type of the boid
   * @param[in] parameters  Parameter of the boid
   */
  MovableBoid(glm::vec3 location, glm::vec3 velocity, BoidType t,
              MovableParameters* parameters);

  /**
   * @brief Constructor of a MovableBoid
   * @param[in] location    Initial location of the boid
   * @param[in] velocity    Initial velocity of the boid
   * @param[in] mass        Mass of the boid
   * @param[in] t           Type of the boid
   * @param[in] parameters  Parameter of the boid
   */
  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass, BoidType t,
              MovableParameters* parameters);

  /**
   * @brief Destructor of MovableBoid
   */
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
  glm::vec3 getVelocity() const;

  /**
   * @brief Getter for the mass of the object
   */
  float getMass() const;

  /**
   * @brief   Getter for the parameter of the object
   * @return  Parameter of the object
   */
  MovableParameters & getParameters() const;

  /**
   * @brief Getter for the state type of the object
   * @return State Type of the object
   */
  StateType getStateType() const;

  /**
   * @brief Set the acceleration field of the class to 0
   */
  void resetAcceleration();

  /**
   * @brief     Update the acceleration of the boid. Save the value
   *            in the acceleration field of the class
   * @param[in] mvB Other movable boid the object is aware of
   */
  void computeAcceleration(const std::vector<MovableBoidPtr> & mvB, const float & dt);

  /**
   * @brief     Update the position and the velocity for the next step in the simulation 
   * @param[in] dt Value of the time step
   */
  void computeNextStep(const float & dt);

  /**
   * @brief     Check if the other boid is in the cone of vision of this
   * @param[in] other     The other boid to check if in range
   * @param[in] distView  The maximum distance viewable by this
   */
	bool canSee(const Boid & other, const float & distView) const;

  /**
   * @brief     Check if the other boid is in the circle of center of the
   *            location of this and of the radius of distView 
   * @param[in] other     The other boid to check if in range
   * @param[in] distView  The radius of the circle
   * @return    true if in the circle, false otherwise
   */
  bool distVision (const Boid & other, const float & distView) const;

  /**
   * @brief     Check if the other boid is the same type of this
   * @param[in] other The other boid
   * @return    true if the other boid is the same type of the, false otherwise
   */
  bool sameSpecies(const Boid & other) const;

  /**
   * @brief     Check if the other boid is in the angle of view of this (distance don't matter)
   * @param[in] other The other boid
   * @return    true if the other boid is in the angle of view of this, false otherwise
   * @warning   If the angle of view of the object is equal to PI, this function might not work
   */
  bool angleVision (const Boid & other) const;

  bool hasLeader() const;

 private:
  glm::vec3 m_velocity; ///< Velocity of the boid
  glm::vec3 m_acceleration; ///< Acceleration of the boid
  float m_mass; ///< Mass of the boid

  MovableState* m_currentState; ///< State of the boid. @seeMovableState
  MovableParameters* m_parameters; ///< Parameter of the boid

  void switchToState(const StateType & stateType);

  /**
   * @brief Contain the rules for a walking boid
   */
  void walkStateHandler();

  /**
   * @brief Contain the rules for a staying boid
   */
  void stayStateHandler();

  /**
   * @brief Contain the rules for a boid finding food
   */
  void findFoodStateHandler();

  /**
   * @brief Contain the rules for a boid to attack
   */
  void attackStateHandler();

  /**
   * @brief Contain the rules for a boid to eat
   */
  void eatStateHandler();

  /**
   * @brief Contain the rules for a boid when he is lost
   */
  void lostStateHandler();

  void sleepStateHandler();

  void fleeStateHandler();

  void findWaterStateHandler();

  void drinkStateHandler();

  void mateStateHandler();

  StateType m_stateType; ///< Save the current state of a boid

  bool hasPrey() const;

  bool closeToPrey() const;

  bool isInGroup() const;

  bool nextToWater() const;

  bool hasASoulMate() const;

  bool isNoLongerMating() const;

  bool preyIsDead() const;

  bool isNight() const;
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