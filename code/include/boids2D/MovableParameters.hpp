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
  float getStamina();
  void staminaIncrease();
  void staminaIncrease(const float & f);
  void staminaDecrease();
  void staminaDecrease(const float & f);
  bool isLowStamina();
  bool isHighStamina();

  // Hunger functions
  float getHunger();
  void hungerIncrease();
  void hungerDecrease();
  bool isLowHunger();

  // Thirst functions
  float getThirst();
  void thirstIncrease();
  void thirstDecrease();

  // Danger functions
  float getDanger();
  void dangerIncrease();
  void dangerDecrease();

  // Affinity functions
  float getAffinity();
  void affinityIncrease();
  void affinityDecrease();

  // Leader functions
  bool isLeader();
  MovableBoidPtr getLeader();
  void setNewLeader(MovableBoidPtr newLeader);

  // Getter of parameters
  float getMaxSpeed();
  float getMaxForce();
  float getRadiusCircleWander();
  float getDistToCircleWander();
  float getDistStartSlowingDown();
  float getDistSeparate();
  float getDistViewCohesion();
  float getAngleView();
  float getDistViewMax();

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