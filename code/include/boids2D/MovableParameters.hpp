#ifndef MOVABLE_PARAMETERS_HPP
#define MOVABLE_PARAMETERS_HPP

#include "MovableBoid.hpp"
class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class MovableParameters
{

public:

	MovableParameters(bool isLeader, MovableBoidPtr m_leader);
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

protected:
	float m_stamina;
	float m_hunger;
	float m_thirst;
	float m_danger;
	float m_affinity;
	MovableBoidPtr m_thisBoid;

	bool m_isLeader;
	MovableBoidPtr m_leader;
};

#endif