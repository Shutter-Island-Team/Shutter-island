#ifndef MOVABLE_PARAMETERS_HPP
#define MOVABLE_PARAMETERS_HPP

#include "MovableBoid.hpp"
class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class MovableParameters
{

public:
	// TODO : Constructor with parameter
	MovableParameters(MovableBoidPtr m_leader);
	void initializeParameters(MovableBoidPtr thisBoid);

	// TODO : add function isLow, isHigh or isHungry, ...
	// TODO : getter are not quite useful here. TODO : to remove

	float getStamina();
	void staminaIncrease();
	void staminaIncrease(float f);
	void staminaDecrease();
	void staminaDecrease(float f);

	float getHunger();
	void hungerIncrease();
	void hungerDecrease();

	float getThirst();
	void thirstIncrease();
	void thirstDecrease();

	float getDanger();
	void dangerIncrease();
	void dangerDecrease();

	float getAffinity();
	void affinityIncrease();
	void affinityDecrease();

	bool isLeader();
	MovableBoidPtr getLeader();
	void setNewLeader(MovableBoidPtr newLeader);

	bool isLowStamina();
	bool isHighStamina();

	float getMaxSpeed();
	float getMaxForce();
	float getRadiusCircleWander();
	float getDistToCircleWander();
	float getDistStartSlowingDown();

	float getDistSeparate();
	float getDistViewCohesion();

	float getAngleView();

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

 private:
  float m_maxSpeed = 3.5f;
  float m_maxForce = 2.0f;

  float m_rCircleWander = 9.0f;
  float m_distToCircle = 3.0f;
  float m_distStartSlowingDown = 5.0f;

  float m_distViewSeparate = 2.0f;
  float m_distViewCohesion = 5.0f;

  float m_angleView = 3*M_PI/4;

};

#endif