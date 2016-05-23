#ifndef MOVABLE_PARAMETERS_HPP
#define MOVABLE_PARAMETERS_HPP

#include "MovableBoid.hpp"
class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class MovableParameters
{

public:

	MovableParameters(MovableBoidPtr m_leader);
	void initializeParameters(MovableBoidPtr thisBoid);

	// TODO : add function isLow, isHigh or isHungry, ...

	float getStamina();
	void staminaIncrease();
	void staminaDecrease();

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
};

#endif