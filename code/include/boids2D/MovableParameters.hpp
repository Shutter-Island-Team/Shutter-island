#ifndef MOVABLE_PARAMETERS_HPP
#define MOVABLE_PARAMETERS_HPP

#include <sstream>

#include "MovableBoid.hpp"
class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class MovableParameters
{
public:
  MovableParameters();

  MovableParameters(float maxSpeed, float maxForce, float angleView,
		float distViewSeparate, float distViewCohesion, float distViewMax,
    float distToLeader);

  MovableParameters(float maxSpeed, float maxForce, float angleView,
		float distViewSeparate, float distViewCohesion, float distViewMax,
    float distToLeader, float distStartSlowingDown, float rCircleWander,
    float distToCircle);
	
  MovableParameters(const std::string & filename);

  MovableParameters(const BoidType & type);

  // Stamina functions
  /**
   * @brief  Getter for the stamina
   * @return Value of the stamina
   * @todo  It may be useless
   */
  float getStamina() const;

  /**
   * @brief Increase the stamina
   * @todo  Review it
   */
  void staminaIncrease();

  /**
   * @brief     Increase the stamina by a certain value
   * @param[in] f Value to modify
   * @todo  Review it
   */
  void staminaIncrease(const float & f);

  /**
   * @brief Decrease the stamina
   * @todo  Review it
   */
  void staminaDecrease();

  /**
   * @brief     Decrease the stamina by a certain value
   * @param[in] f Value to modify
   * @todo  Review it
   */
  void staminaDecrease(const float & f);
  
  /**
   * @brief Check if the stamina is high. "high" depends on the parameter
   * @return true if the stamina is high, false otherwise
   */
  bool isHighStamina() const;

  // Hunger functions
  /**
   * @brief Getter for the hunger
   * @return Value of the hunger
   */
  float getHunger() const;

  /**
   * @brief Increase the hunger value
   * @todo  Review it
   */
  void hungerIncrease();

  /**
   * @brief Decrease the hunger value
   * @todo  Review it
   */
  void hungerDecrease();

  /**
   * @brief Check if the the boid is hungry
   * @return true if the boid is hungry, false otherwise
   */
  bool isHungry() const;

  // Thirst functions
  /**
   * @brief Getter for the Thirst
   */
  float getThirst() const;
  void thirstIncrease();
  void thirstDecrease();

  // Danger functions
  float getDanger() const;
  void dangerIncrease();
  void dangerDecrease();

  // Affinity functions
  float getAffinity() const;
  void affinityIncrease();
  void affinityDecrease();

  // Leader functions
  bool isLeader() const;
  MovableBoidPtr getLeader() const;
  void setNewLeader(MovableBoidPtr newLeader);

  // Getter of parameters
  float getMaxSpeed() const;
  float getMaxForce() const;
  float getRadiusCircleWander() const;
  float getDistToCircleWander() const;
  float getDistStartSlowingDown() const;
  float getDistSeparate() const;
  float getDistViewCohesion() const;
  float getAngleView() const;
  float getDistViewMax() const;

  bool isInDanger() const;
  bool isNotInDanger() const;
  bool isThursty() const;
  bool isTired() const;
  bool isNotTired() const;
  bool isNotThursty() const;

  float getDistToLeader() const;

protected:
  float m_stamina; ///< Value in [0, 100] to describe the stamina of a boid
  float m_hunger; ///< Value in [0, 100] to describe the hunger of a boid
  float m_thirst; ///< Value in [0, 100] to describe the thirst of a boid
  float m_danger; ///< Value in [0, 100] to describe the feel of danger of a boid
  float m_affinity; ///< Value in [0, 100] to describe the affinity of a boid
  MovableBoidPtr m_thisBoid;
  MovableBoidPtr m_leader;

 private:
  float m_maxSpeed; ///< Maximum speed of the boid
  float m_maxForce; ///< Maximum force of the boid

  float m_rCircleWander;
  float m_distToCircle;
  float m_distStartSlowingDown;

  float m_distViewSeparate;
  float m_distViewCohesion;

  float m_angleView;

  float m_distViewMax;

  float m_lowStaminaValue;
  float m_highStaminaValue;

  float m_lowHungerValue;

  float m_distToLeader;

  // Only allow MovableBoid to use associateBoid
  friend class MovableBoid;
  void associateBoid(MovableBoidPtr thisBoid);
};

#endif