#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "../../include/rapidjson/document.h"
#include "../../include/Utils.hpp"
#include "../../include/boids2D/MovableParameters.hpp"

MovableParameters::MovableParameters()
	: MovableParameters(3.5f, 7.0f, 2.0f, 3*M_PI/4, 2.0f, 4.0f, 5.0f, 1.0f, 1.5f, 0.8f, 20.0f)
{

}

MovableParameters::MovableParameters(float maxSpeedWalk,
		float maxSpeedRun, float maxForce,
		float angleView, float distSeparate, float distCohesion,
		float distViewMax, float distToLeader, float distSeeAhead,
		float distAttack, float distMaxToLeader) : 
	MovableParameters(maxSpeedWalk, maxSpeedRun, maxForce, angleView, distSeparate,
						distCohesion, distViewMax, distToLeader,
						distSeeAhead, distAttack, distMaxToLeader, 5.0f, 9.0f, 3.0f)
{

}

MovableParameters::MovableParameters(float maxSpeedWalk, float maxSpeedRun, float maxForce,
	float angleView, float distSeparate, float distCohesion, float distViewMax,
	float distToLeader, float distSeeAhead, float distAttack, float distMaxToLeader, float distStartSlowingDown, 
	float rCircleWander, float distToCircle) :
	m_maxSpeedWalk(maxSpeedWalk), m_maxSpeedRun(maxSpeedRun), m_maxForce(maxForce), m_angleView(angleView),
	m_distSeparate(distSeparate), m_distCohesion(distCohesion),
	m_distViewMax(distViewMax), m_distToLeader(distToLeader),
	m_distStartSlowingDown(distStartSlowingDown), m_distSeeAhead(distSeeAhead), m_distAttack(distAttack),
	m_distMaxToLeader(distMaxToLeader), m_rCircleWander(rCircleWander), m_distToCircle(distToCircle), 
	m_danger(0.0f), m_affinity(0.0f), m_stamina(random(55,99)), m_hunger(random(55,99)), m_thirst(random(55,99))
{
	rapidjson::Document d;

	std::ifstream t("../boidData/VariableParameters.json");
	std::string str;

	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
	            std::istreambuf_iterator<char>());	

	d.Parse(str.c_str());

	m_lowStaminaValue = d["Boundaries"]["Stamina"]["low"].GetDouble();
	m_highStaminaValue = d["Boundaries"]["Stamina"]["high"].GetDouble();

	m_lowHungerValue = d["Boundaries"]["Hunger"]["low"].GetDouble();
	m_highHungerValue = d["Boundaries"]["Hunger"]["high"].GetDouble();

	m_lowThirstValue = d["Boundaries"]["Thirst"]["low"].GetDouble();
	m_highThirstValue = d["Boundaries"]["Thirst"]["high"].GetDouble();

	m_lowDangerValue = d["Boundaries"]["Danger"]["low"].GetDouble();
	m_highDangerValue = d["Boundaries"]["Danger"]["high"].GetDouble();

	m_hungerIncCoeff = d["Coefficient"]["Global state"]["Hunger"].GetDouble();
	m_hungerDecCoeffWalk = d["Coefficient"]["WalkState"]["Hunger"].GetDouble();
	m_hungerDecCoeffRun = d["Coefficient"]["RunState"]["Hunger"].GetDouble();

	m_staminaIncCoeff = d["Coefficient"]["Global state"]["Stamina"].GetDouble();
	m_staminaDecCoeffWalk = d["Coefficient"]["WalkState"]["Stamina"].GetDouble();
	m_staminaDecCoeffRun = d["Coefficient"]["RunState"]["Stamina"].GetDouble();

	m_thirstIncCoeff = d["Coefficient"]["Global state"]["Thirst"].GetDouble();
	m_thirstDecCoeffWalk = d["Coefficient"]["WalkState"]["Thirst"].GetDouble();
	m_thirstDecCoeffRun = d["Coefficient"]["RunState"]["Thirst"].GetDouble();

	m_affinityIncCoeff = d["Coefficient"]["Global state"]["AffinityIncrease"].GetDouble();
	m_affinityDecCoeff = d["Coefficient"]["Global state"]["AffinityDecrease"].GetDouble();

	m_dangerIncCoeff = d["Coefficient"]["Global state"]["DangerIncrease"].GetDouble();
	m_dangerDecCoeff = d["Coefficient"]["Global state"]["DangerDecrease"].GetDouble();
}

MovableParameters::MovableParameters( const std::string & filename )
{
	rapidjson::Document d;

	std::ifstream t(filename);
	std::string str;

	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
	            std::istreambuf_iterator<char>());	

	d.Parse(str.c_str());

	*this = MovableParameters(d["maxSpeedWalk"].GetDouble(),
		d["maxSpeedRun"].GetDouble(),
		d["maxForce"].GetDouble(), d["angleView"].GetDouble(),
		d["distSeparate"].GetDouble(), d["distCohesion"].GetDouble(),
		d["distViewMax"].GetDouble(), d["distToLeader"].GetDouble(),
		d["distSeeAhead"].GetDouble(), d["distAttack"].GetDouble(),
		d["distMaxToLeader"].GetDouble(), d["distStartSlowingDown"].GetDouble(),
		d["rCircleWander"].GetDouble(), d["distToCircle"].GetDouble());
}

MovableParameters::MovableParameters(const BoidType & type)
{
	switch (type) {
		case RABBIT:
			*this = MovableParameters( "../boidData/RabbitParameters.json" ); 
			break;
		case WOLF:
			*this = MovableParameters( "../boidData/WolfParameters.json" );
			break;
		default:
			std::cerr << "Unknown animal" << std::endl;
			break;
	}
}

float MovableParameters::getStamina() const
{
	return m_stamina;
}

void MovableParameters::staminaIncrease()
{
	m_stamina = fmin(m_stamina + m_staminaIncCoeff, 100.0f);
}

void MovableParameters::staminaDecreaseWalk()
{
	m_stamina = fmax(m_stamina - m_staminaDecCoeffWalk, 0.0f);
}

void MovableParameters::staminaDecreaseRun()
{
	m_stamina = fmax(m_stamina - m_staminaDecCoeffRun, 0.0f);
}

float MovableParameters::getHunger() const
{
	return m_hunger;
}

void MovableParameters::hungerIncrease()
{
	m_hunger = fmin(m_hunger + m_hungerIncCoeff, 100.0f);
}

void MovableParameters::hungerDecreaseWalk()
{
	m_hunger = fmax(m_hunger - m_hungerDecCoeffWalk, 0.0f);
}

void MovableParameters::hungerDecreaseRun()
{
	m_hunger = fmax(m_hunger - m_hungerDecCoeffRun, 0.0f);
}

float MovableParameters::getThirst() const
{
	return m_thirst;
}

void MovableParameters::thirstIncrease()
{
	m_thirst = fmin(m_thirst + m_thirstIncCoeff, 100.0f);
}

void MovableParameters::thirstDecreaseWalk()
{
	m_thirst = fmax(m_thirst - m_thirstDecCoeffWalk, 0.0f);
}

void MovableParameters::thirstDecreaseRun()
{
	m_thirst = fmax(m_thirst - m_thirstDecCoeffRun, 0.0f);
}

float MovableParameters::getDanger() const
{
	return m_danger;
}

void MovableParameters::dangerIncrease()
{
	m_danger = fmin(m_danger + m_dangerIncCoeff, 100.0f);
}

void MovableParameters::dangerDecrease()
{
	m_danger = fmax(m_danger - m_dangerDecCoeff, 0.0f);
}

float MovableParameters::getAffinity() const
{
	return m_affinity;
}

void MovableParameters::affinityIncrease()
{
	m_affinity = fmin(m_affinity + m_affinityIncCoeff, 100.0f);
}

void MovableParameters::affinityDecrease()
{
	m_affinity = fmax(m_affinity - m_affinityDecCoeff, 0.0f);
}

bool MovableParameters::isTired() const
{
	return m_stamina <= m_lowStaminaValue;
}

bool MovableParameters::isHighStamina() const
{
	return m_stamina >= m_highStaminaValue;
}

bool MovableParameters::isHungry() const
{
	return m_hunger <= m_lowHungerValue;
}

bool MovableParameters::isStarving() const
{
	return m_hunger <= (m_lowHungerValue / 2);
}

bool MovableParameters::isNotHungry() const
{
	return m_hunger >= m_highHungerValue;
}

float MovableParameters::getMaxSpeedWalk() const
{
	return m_maxSpeedWalk;
}

float MovableParameters::getMaxSpeedRun() const
{
	return m_maxSpeedRun;
}

float MovableParameters::getMaxForce() const
{
	return m_maxForce;
}

float MovableParameters::getRadiusCircleWander() const
{
	return m_rCircleWander;
}

float MovableParameters::getDistToCircleWander() const
{
	return m_distToCircle;
}

float MovableParameters::getDistStartSlowingDown() const
{
	return m_distStartSlowingDown;
}

float MovableParameters::getDistSeparate() const
{
	return m_distSeparate;
}

float MovableParameters::getDistViewCohesion() const
{
	return m_distCohesion;
}

float MovableParameters::getAngleView() const
{
	return m_angleView;
}

float MovableParameters::getDistViewMax() const
{
	return m_distViewMax;
}

bool MovableParameters::isInDanger() const
{
	return m_danger >= m_highDangerValue;	
}

bool MovableParameters::isThirsty() const
{
	return m_thirst <= m_lowThirstValue;
}

bool MovableParameters::isNotThirsty() const
{
	return m_thirst >= m_highThirstValue;	
}

bool MovableParameters::isNotTired() const
{
	return m_stamina >= m_highStaminaValue;	
}

bool MovableParameters::isNotInDanger() const
{
	return m_danger <= m_lowDangerValue;	
}

float MovableParameters::getDistToLeader() const
{
	return m_distToLeader;
}

float MovableParameters::getDistSeeAhead() const
{
	return m_distSeeAhead;
}

float MovableParameters::getDistAttack() const
{
	return m_distAttack;
}

bool MovableParameters::isHighAffinity() const
{
	return m_affinity == 100.0f;
}

void MovableParameters::resetAffinity()
{
	m_affinity = 0.0f;
}

