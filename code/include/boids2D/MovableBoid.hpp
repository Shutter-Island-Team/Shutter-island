#ifndef MOVABLE_BOID_HPP
#define MOVABLE_BOID_HPP

#include "Boid.hpp"
#include "StateType.hpp"
#include <vector>
#include <cmath>

#include "MovableState.hpp"
#include "MovableParameters.hpp"
#include "BoidsManager.hpp"
#include "RootedBoid.hpp"

class BoidsManager;

class MovableState;
typedef std::shared_ptr<MovableState> MovableStatePtr;

class MovableParameters;
typedef std::shared_ptr<MovableParameters> MovableParametersPtr;

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
	MovableBoid(glm::vec3 location, BoidType t, MovableParametersPtr parameters);

  /**
   * @brief Constructor of a MovableBoid
   * @param[in] location    Initial location of the boid
   * @param[in] velocity    Initial velocity of the boid
   * @param[in] t           Type of the boid
   * @param[in] parameters  Parameter of the boid
   */
  MovableBoid(glm::vec3 location, glm::vec3 velocity, BoidType t,
              MovableParametersPtr parameters);

  /**
   * @brief Constructor of a MovableBoid
   * @param[in] location    Initial location of the boid
   * @param[in] velocity    Initial velocity of the boid
   * @param[in] mass        Mass of the boid
   * @param[in] t           Type of the boid
   * @param[in] parameters  Parameter of the boid
   */
  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass, BoidType t,
              MovableParametersPtr parameters);

 protected:
  /**
   * @brief Constructor of a MovableBoid
   * @param[in] location    Initial location of the boid
   * @param[in] velocity    Initial velocity of the boid
   * @param[in] mass        Mass of the boid
   * @param[in] t           Type of the boid
   * @param[in] parameters  Parameter of the boid
   * @param[in] amountFood  Amount of food in the boid
   */
  MovableBoid(glm::vec3 location, glm::vec3 velocity, float mass, BoidType t,
              MovableParametersPtr parameters, int amountFood);
 public:

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
  MovableParametersPtr getParameters() const;

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
   * @param[in] boidsManager The boid's manager
   * @param[in] dt           Time step
   * @param[in] updateTick   True if the current tick should have update of state of boids
   */
  void computeAcceleration(const BoidsManager & boidsManager, const float & dt, const bool & updateTick);

  /**
   * @brief     Update the position and the velocity for the next step in the simulation 
   * @param[in] dt Value of the time step
   */
  const glm::vec3 computeNextStep(const float & dt);

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

  /**********************************
          Leader methods
  ***********************************/
  /**
   * @brief Check if the current boid is a leader
   * @return True if the current boid is a leader, false otherwise
   */
  bool isLeader() const;

  /**
   * @brief Getter for the leader of the boid
   * @return Return the leader of the boid
   */
  MovableBoidPtr getLeader() const;

  /**
   * @brief Setter for the leader of the boid
   * @param[in] newLeader The new leader of the boid
   */
  void setNewLeader(MovableBoidPtr newLeader);

  /**
   * @brief   Check if the boid has a leader
   * @return  true if the boid has a leader, false otherwise
   */
  bool hasLeader() const;

  /**
   * @brief Setter for the movable prey of the boid
   * @param[in] boid The movable prey of the boid
   */
  void setMovablePrey(const MovableBoidPtr & boid);

  /**
   * @brief Getter for the movable prey of the boid
   * @return Returns the movable prey of the boid
   */
  MovableBoidPtr getMovablePrey() const;

  /**
   * @brief Setter for the rooted prey of the boid
   * @param[in] boid The rooted prey of the boid
   */
  void setRootedPrey(const RootedBoidPtr & boid);

  /**
   * @brief Getter for the rooted prey of the boid
   * @return Returns the rooted prey of the boid
   */
  RootedBoidPtr getRootedPrey() const;

  /**
   * @brief Setter for the hunter of the boid
   * @param[in] boid The hunter of the boid
   */
  void setHunter(const MovableBoidPtr & boid);

  /**
   * @brief Getter for the hunter of the boid
   * @return Returns the hunter
   */
  MovableBoidPtr getHunter() const;

  /**
   * @brief Getter for the type of boid which are predator of the current boid
   * @return Returns the type of the predator
   */
  BoidType getPredatorType() const;

  /**
   * @brief     Set the predator type for the boid
   * @param[in] predator Type of the boid to be the predator
   */
  void setPredatorType(const BoidType & predator);

  /**
   * @brief   Getter to know if the boid is dead
   * @return  True if the boid is dead, false otherwise
   */
  bool isDead() const;

  /**
   * @brief Set the dead status of the boid
   */
  void die();

  /**
   * @brief Update the boid, looking at its coefficient of living 
   */
  void updateDeadStatus();

  /**
   * @brief   Getter for the mate of the boid
   * @return  Returns the mate of the boid
   */
  MovableBoidPtr getMate() const;

  /**
   * @brief  Setter for the mate of the boid
   */
  void setMate(const MovableBoidPtr & mate);

  /**
   * @brief     Updates the water target
   * @param[in] b Reference of the boidManager to ask for the source position
   */
  void updateWaterTarget(const BoidsManager & b);

  /**
   * @brief Getter for the position of the water
   * @return Return the position of the water
   */
  glm::vec3 getWaterTarget() const;

 private:
  glm::vec3 m_velocity; ///< Velocity of the boid
  glm::vec3 m_acceleration; ///< Acceleration of the boid
  float m_mass; ///< Mass of the boid

  MovableStatePtr m_currentState; ///< State of the boid
  MovableParametersPtr m_parameters; ///< Parameter of the boid

  MovableBoidPtr m_movablePrey; ///< Movable prey of the boid. (If exist cannot have a rooted prey)
  RootedBoidPtr m_rootedPrey; ///< Rooted prey of the boid. (If exist cannot have a movable prey)

  MovableBoidPtr m_hunter; ///< Hunter of the boid
  MovableBoidPtr m_leader; ///< Leader of the boid

  MovableBoidPtr m_mate; ///< Mate of the boid

  BoidType m_predator; ///< Type of the predator

  bool m_isDead; ///< Boolean to know if the boid is dead

  glm::vec3 m_waterTarget; ///< Position of the water source

  /**
   * @brief     Make all the change when a boid get to the new state stateType
   * @param[in] stateType     The state the boid change to
   * @param[in] boidsManager  Reference of the boidsManager
   */
  void switchToState(const StateType & stateType, const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a walking boid
   */
  void walkStateHandler(const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a staying boid
   */
  void stayStateHandler(const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a boid finding food
   */
  void findFoodStateHandler(const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a boid to attack
   */
  void attackStateHandler(const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a boid to eat
   */
  void eatStateHandler(const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a boid when he is lost
   */
  void lostStateHandler(const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a boid when he sleeps
   */
  void sleepStateHandler(const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a boid when he flees
   */
  void fleeStateHandler(const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a boid when he is looking for water
   */
  void findWaterStateHandler(const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a boid when he drinks
   */
  void drinkStateHandler(const BoidsManager & boidsManager);

  /**
   * @brief Contain the rules for a boid when he mates
   */
  void mateStateHandler(const BoidsManager & boidsManager);

  StateType m_stateType; ///< Save the current state of a boid

  /**
   * @brief   Check if the boid has a movable prey or a rooted prey 
   * @return  true if the boid has a prey, false otherwise
   */
  bool hasPrey() const;

  /**
   * @brief   Check if the boid is close to its prey (can touch it)
   * @return  true if the boid is close to the prey, false otherwise
   */
  bool closeToPrey() const;

  /**
   * @brief   Check if the boid is in a group
   * @return  true if the boid is in a group, false otherwise
   */
  bool isInGroup() const;

  /**
   * @brief     Check if the boid is next to a source of water (can touch it)
   * @param[in] boidsManager The boids manager
   * @return    true if the boid is next to a source of water, false otherwise
   */
  bool nextToWater(const BoidsManager & boidsManager) const;

  /**
   * @brief   Check if the boid has a soulmate
   * @return  true if the boid has a soulmate, false otherwise
   */
  bool hasSoulMate() const;

  /**
   * @brief   Check if the boid is not mating
   * @return  true if the boid is not mating, false otherwise
   */
  bool isNoLongerMating() const;

  /**
   * @brief   Check if the prey of the boid is dead
   * @return  true if the its prey is dead, false otherwise
   */
  bool preyIsDead() const;

  /**
   * @brief   Check if it is night time
   * @return  true if is night time, false otherwise
   */
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