#ifndef MOVABLE_PARAMETERS_HPP
#define MOVABLE_PARAMETERS_HPP

#include "BoidType.hpp"

/**
 * @class MovableParameters
 * @brief Class with all the parameters of a boid and method to check their value
 */
class MovableParameters
{
public:
  /**
   * @brief Constructor for MovableParameters
   */
  MovableParameters();

  /**
   * @brief     Constructor for MovableParameters
   * @param[in] maxSpeed      Initial max speed
   * @param[in] maxForce      Initial max force
   * @param[in] angleView     Angle of view
   * @param[in] distSeparate  Distance of separation 
   * @param[in] distCohesion  Distance of cohesion
   * @param[in] distViewMax   Distance of maximum view
   * @param[in] distToLeader  Distance to keep with the leader of the group
   */
  MovableParameters(float maxSpeed, float maxForce, float angleView,
		float distSeparate, float distCohesion, float distViewMax,
    float distToLeader, float distSeeAhead);

  /**
   * @brief     Constructor for MovableParameters
   * @param[in] maxSpeed              Initial max speed
   * @param[in] maxForce              Initial max force
   * @param[in] angleView             Angle of view
   * @param[in] distSeparate          Distance of separation 
   * @param[in] distCohesion          Distance of cohesion
   * @param[in] distViewMax           Distance of maximum view
   * @param[in] distToLeader          Distance to keep with the leader of the group
   * @param[in] distStartSlowingDown  Distance before slowing down for arrive behavior
   * @param[in] rCircleWander         Radius of the circle for wander behavior
   * @param[in] distToCircle          Distance between the boid and the wander circle   
   */
  MovableParameters(float maxSpeed, float maxForce, float angleView,
		float distSeparate, float distCohesion, float distViewMax,
    float distToLeader, float distSeeAhead, float distStartSlowingDown, float rCircleWander,
    float distToCircle);
	
  /**
   * @brief     Constructor for MovableParameters from a file
   * @param[in] filename Name of the file
   */
  MovableParameters(const std::string & filename);

  /**
   * @brief     Constructor for MovableParaeters from a type.
   *            (Call the constructor from a file)
   * @param[in] type Type of the requested boid
   */
  MovableParameters(const BoidType & type);

  /**********************************
          Stamina methods
  ***********************************/
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

  /**
   * @brief   Check if the boid is tired
   * @return  true if the boid is tired, false otherwise
   */
  bool isTired() const;

  /**
   * @brief   Check if the boid not tired
   * @return  true if the boid is not tired, false otherwise
   */
  bool isNotTired() const;



  /**********************************
          Hunger methods
  ***********************************/
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

  /**********************************
          Thirst methods
  ***********************************/
  /**
   * @brief Getter for the thirst
   */
  float getThirst() const;

  /**
   * @brief Increase the thirsst value
   * @todo  Review it
   */
  void thirstIncrease();

  /**
   * @brief Decrease the thirst value
   * @todo  Review it
   */
  void thirstDecrease();

  /**
   * @brief Check if the boid is thristy
   * @return true if the boid is thristy, false otherwise
   */
  bool isThirsty() const;
  bool isNotThirsty() const;

  /**********************************
          Danger methods
  ***********************************/
  // Danger functions
  float getDanger() const;
  void dangerIncrease();
  void dangerDecrease();
  bool isInDanger() const;
  bool isNotInDanger() const;

  /**********************************
          Affinity methods
  ***********************************/
  // Affinity functions
  float getAffinity() const;
  void affinityIncrease();
  void affinityDecrease();

  /**********************************
          Getter methods
  ***********************************/
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
  float getDistToLeader() const;

  float getDistSeeAhead() const;


protected:
  float m_stamina; ///< Value in [0, 100] to describe the stamina of a boid
  float m_hunger; ///< Value in [0, 100] to describe the hunger of a boid
  float m_thirst; ///< Value in [0, 100] to describe the thirst of a boid
  float m_danger; ///< Value in [0, 100] to describe the feel of danger of a boid
  float m_affinity; ///< Value in [0, 100] to describe the affinity of a boid

 private:
  float m_maxSpeed; ///< Maximum speed of the boid
  float m_maxForce; ///< Maximum force of the boid

  float m_rCircleWander;
  float m_distToCircle;
  float m_distStartSlowingDown;

  float m_distSeparate;
  float m_distCohesion;

  float m_angleView;

  float m_distViewMax;

  float m_lowStaminaValue;
  float m_highStaminaValue;

  float m_lowHungerValue;

  float m_distToLeader;

  float m_distSeeAhead; ///< Distance to see ahead the obstacle
};

#endif