#ifndef MOVABLE_PARAMETERS_HPP
#define MOVABLE_PARAMETERS_HPP

#include <sstream>

#include "MovableBoid.hpp"
class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class MovableParameters
{

public:
	// TODO : Constructor with parameter
	MovableParameters();

	MovableParameters(float maxSpeed, float maxForce, float angleView, float distViewSeparate, float distViewCohesion);

	MovableParameters(float maxSpeed, float maxForce, float angleView, float distViewSeparate, float distViewCohesion, float distStartSlowingDown,float rCircleWander, float distToCircle);
	
	MovableParameters( const std::string & filename );

	MovableParameters(BoidType type);

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

	friend std::istream& operator>>(std::istream &in, MovableParameters &param);

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
  float m_maxSpeed;
  float m_maxForce;

  float m_rCircleWander;
  float m_distToCircle;
  float m_distStartSlowingDown;

  float m_distViewSeparate;
  float m_distViewCohesion;

  float m_angleView;

};

#endif