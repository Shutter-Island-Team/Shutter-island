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
		float distViewSeparate, float distViewCohesion, float distViewMax);

  MovableParameters(float maxSpeed, float maxForce, float angleView,
		float distViewSeparate, float distViewCohesion, float distViewMax,
		float distStartSlowingDown,float rCircleWander, float distToCircle);
	
  MovableParameters(const std::string & filename);

  MovableParameters(const BoidType & type);

  // Stamina functions
  float getStamina() const;
  void staminaIncrease();
  void staminaIncrease(const float & f);
  void staminaDecrease();
  void staminaDecrease(const float & f);
  bool isHighStamina() const;

  // Hunger functions
  float getHunger() const;
  void hungerIncrease();
  void hungerDecrease();
  bool isHungry() const;

  // Thirst functions
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

protected:
  float m_stamina;
  float m_hunger;
  float m_thirst;
  float m_danger;
  float m_affinity;
  MovableBoidPtr m_thisBoid;
  MovableBoidPtr m_leader;

  float m_lowStaminaValue;
  float m_highStaminaValue;

  float m_lowHungerValue;

 private:
  float m_maxSpeed;
  float m_maxForce;

  float m_rCircleWander;
  float m_distToCircle;
  float m_distStartSlowingDown;

  float m_distViewSeparate;
  float m_distViewCohesion;

  float m_angleView;

  float m_distViewMax;

  // Only allow MovableBoid to use associateBoid
  friend class MovableBoid;
  void associateBoid(MovableBoidPtr thisBoid);
};

#endif