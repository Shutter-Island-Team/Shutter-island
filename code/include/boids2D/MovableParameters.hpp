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
   * @param[in] distSeeAhead  Distance to see ahead the obstacles
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
   * @param[in] distSeeAhead          Distance to see ahead the obstacles
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
   * @brief Increase the hunger
   * @todo  Review it
   */
  void hungerIncrease();

  /**
   * @brief     Increase the hunger by a certain value
   * @param[in] f Value to modify
   * @todo  Review it
   */
  void hungerIncrease(const float & f);

  /**
   * @brief Decrease the hunger
   * @todo  Review it
   */
  void hungerDecrease();

  /**
   * @brief     Decrease the hunger by a certain value
   * @param[in] f Value to modify
   * @todo  Review it
   */
  void hungerDecrease(const float & f);

  /**
   * @brief Check if the the boid is hungry
   * @return true if the boid is hungry, false otherwise
   */
  bool isHungry() const;
  bool isNotHungry() const;

  /**********************************
          Thirst methods
  ***********************************/
  /**
   * @brief Getter for the thirst
   */
  float getThirst() const;

  /**
   * @brief Increase the thirst
   * @todo  Review it
   */
  void thirstIncrease();

  /**
   * @brief     Increase the thirst by a certain value
   * @param[in] f Value to modify
   * @todo  Review it
   */
  void thirstIncrease(const float & f);

  /**
   * @brief Decrease the thirst
   * @todo  Review it
   */
  void thirstDecrease();

  /**
   * @brief     Decrease the thirst by a certain value
   * @param[in] f Value to modify
   * @todo  Review it
   */
  void thirstDecrease(const float & f);

  /**
   * @brief Check if the boid is thristy
   * @return true if the boid is thristy, false otherwise
   */
  bool isThirsty() const;
  bool isNotThirsty() const;

  /**********************************
          Danger methods
  ***********************************/
  /**
   * @brief Getter for the danger value
   * @return Returns the value of danger
   * @todo Check if it is still useful
   */
  float getDanger() const;

  /**
   * @brief Increment the danger
   */
  void dangerIncrease();

  /**
   * @brief Decrement the danger
   */
  void dangerDecrease();

  /**
   * @brief Check if the boid is in danger
   * @return True if the boid is in danger, false otherwise
   */
  bool isInDanger() const;

  /**
   * @brief Check if the boid is not in danger
   * @return True if the boid is not in danger, false otherwise
   */
  bool isNotInDanger() const;

  /**********************************
          Affinity methods
  ***********************************/
  /**
   * @brief Getter for the affinity value
   * @return Returns the value of affinity
   * @todo Check if it is still useful
   */
  float getAffinity() const;

  /**
   * @brief Increase the affinity
   * @todo  Review it
   */
  void affinityIncrease();

  /**
   * @brief     Increase the affinity by a certain value
   * @param[in] f Value to modify
   * @todo  Review it
   */
  void affinityIncrease(const float & f);

  /**
   * @brief Decrease the affinity
   * @todo  Review it
   */
  void affinityDecrease();

  /**
   * @brief     Decrease the affinity by a certain value
   * @param[in] f Value to modify
   * @todo  Review it
   */
  void affinityDecrease(const float & f);

  /**********************************
          Getter methods
  ***********************************/
  /**
   * @brief Getter for the maximum speed value
   * @return Returns the maximum speed value
   */
  float getMaxSpeed() const;

  /**
   * @brief Getter for the maximum force value
   * @return Returns the maximum force value
   */
  float getMaxForce() const;

  /**
   * @brief Getter for the radius circle wander
   * @return Returns the radius circle wander
   */
  float getRadiusCircleWander() const;

  /**
   * @brief Getter for the maximum speed value
   * @return Returns the maximum speed value
   */
  float getDistToCircleWander() const;

  /**
   * @brief Getter for the distance before slowing down for arrive behavior
   * @return Returns the distance before slowing down for arrive behavior
   */
  float getDistStartSlowingDown() const;

  /**
   * @brief Getter for the distance of separation
   * @return Returns the distance of separation
   */
  float getDistSeparate() const;

  /**
   * @brief Getter for the distance of cohesion
   * @return Returns the distance of cohesion
   */
  float getDistViewCohesion() const;

  /**
   * @brief Getter for the angle of vision
   * @return Returns the angle of vision
   */
  float getAngleView() const;

  /**
   * @brief Getter for the maximum distance of view
   * @return Returns the maximum distance of view
   */
  float getDistViewMax() const;

  /**
   * @brief Getter for the dist to leader to respect
   * @return Returns the dist to leader to respect
   */
  float getDistToLeader() const;

  /**
   * @brief Getter for the vision of ahead for obstacles
   * @return Returns the vision of ahead for obstacles
   */
  float getDistSeeAhead() const;


 private:
  float m_stamina; ///< Value in [0, 100] to describe the stamina of a boid
  float m_hunger; ///< Value in [0, 100] to describe the hunger of a boid
  float m_thirst; ///< Value in [0, 100] to describe the thirst of a boid
  float m_danger; ///< Value in [0, 100] to describe the feel of danger of a boid
  float m_affinity; ///< Value in [0, 100] to describe the affinity of a boid

  float m_maxSpeed; ///< Maximum speed of the boid
  float m_maxForce; ///< Maximum force of the boid

  float m_rCircleWander; ///< Radius of the wander circle 
  float m_distToCircle; ///< Distance between the boid and the center of the wander circle
  float m_distStartSlowingDown; ///< Distance before start slowing down

  float m_distSeparate; ///< Distance of separation
  float m_distCohesion; ///< Distance of cohesion

  float m_angleView; ///< Angle of vision

  float m_distViewMax; ///< Distance of the maximum view

  float m_lowStaminaValue; ///< Value of the low stamina
  float m_highStaminaValue; ///< Value of the high stamina

  float m_lowHungerValue; ///< Value of the low hunger
  float m_highHungerValue; ///< Value of the high hunger

  float m_lowThirstValue; ///< Value of the low thirst
  float m_highThirstValue; ///< Value of the low thirst

  float m_distToLeader; ///< Distance to respect behind the leader

  float m_distSeeAhead; ///< Distance to see ahead the obstacle
};

#endif