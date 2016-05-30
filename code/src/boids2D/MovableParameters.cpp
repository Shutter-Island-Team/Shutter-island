#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "../../include/rapidjson/document.h"
#include "../../include/Utils.hpp"
#include "../../include/boids2D/MovableParameters.hpp"

MovableParameters::MovableParameters()
	: MovableParameters(3.5f, 2.0f, 3*M_PI/4, 2.0f, 4.0f, 5.0f, 1.0f, 1.5f)
{

}

MovableParameters::MovableParameters(float maxSpeed, float maxForce,
		float angleView, float distSeparate, float distCohesion,
		float distViewMax, float distToLeader, float distSeeAhead) : 
	MovableParameters(maxSpeed, maxForce, angleView, distSeparate,
						distCohesion, distViewMax, distToLeader,
						distSeeAhead, 5.0f, 9.0f, 3.0f)
{

}

MovableParameters::MovableParameters(float maxSpeed, float maxForce,
	float angleView, float distSeparate, float distCohesion, float distViewMax,
	float distToLeader, float distSeeAhead, float distStartSlowingDown,float rCircleWander, float distToCircle) :
	m_maxSpeed(maxSpeed), m_maxForce(maxForce), m_angleView(angleView),
	m_distSeparate(distSeparate), m_distCohesion(distCohesion),
	m_distViewMax(distViewMax), m_distToLeader(distToLeader),
	m_distStartSlowingDown(distStartSlowingDown), m_distSeeAhead(distSeeAhead),
	m_rCircleWander(rCircleWander), m_distToCircle(distToCircle), m_danger(0.0f), m_affinity(0.0f), 
	m_stamina(random(1,99)), m_hunger(random(1,99)), m_thirst(random(1,99)),
	m_lowStaminaValue(25.0f), m_highStaminaValue(80.0f), m_lowHungerValue(35.0f), m_highHungerValue(80.0f),
	m_lowThirstValue(35.0f), m_highThirstValue(80.0f)
{

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

	*this = MovableParameters(d["maxSpeed"].GetDouble(),
		d["maxForce"].GetDouble(), d["angleView"].GetDouble(),
		d["distSeparate"].GetDouble(), d["distCohesion"].GetDouble(),
		d["distViewMax"].GetDouble(), d["distToLeader"].GetDouble(),
		d["distSeeAhead"].GetDouble(), d["distStartSlowingDown"].GetDouble(),
		d["rCircleWander"].GetDouble(), d["distToCircle"].GetDouble());
}

MovableParameters::MovableParameters(const BoidType & type)
{
	switch (type) {
		case RABBIT:
			std::cerr << "Read rabbit data from file" << std::endl;
			*this = MovableParameters( "../boidData/RabbitParameters.json" ); 
			break;
		case WOLF:
			std::cerr << "Read wolf data from file" << std::endl;
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
	staminaIncrease(0.1f);
}

void MovableParameters::staminaIncrease(const float & f)
{
	m_stamina = fmin(m_stamina + f, 100.0f);
}

void MovableParameters::staminaDecrease()
{
	staminaDecrease(0.1f);
}

void MovableParameters::staminaDecrease(const float & f)
{
	m_stamina = fmax(m_stamina - f, 0.0f);
}

float MovableParameters::getHunger() const
{
	return m_hunger;
}

void MovableParameters::hungerIncrease()
{
	hungerIncrease(0.1f);
}

void MovableParameters::hungerIncrease(const float & f)
{
	///< @todo : improve with maybe a better function
	m_hunger = fmin(m_hunger + f, 100.0f);
}

void MovableParameters::hungerDecrease()
{
	hungerDecrease(0.1f);	
}

void MovableParameters::hungerDecrease(const float & f)
{
	///< @todo : improve with maybe a better function
	m_hunger = fmax(m_hunger - f, 0.0f);
}

float MovableParameters::getThirst() const
{
	return m_thirst;
}

void MovableParameters::thirstIncrease()
{
	thirstIncrease(0.1f);
}

void MovableParameters::thirstIncrease(const float & f)
{
	///< @todo : improve with maybe a better function
	m_thirst = fmin(m_thirst + f, 100.0f);
}

void MovableParameters::thirstDecrease()
{
	thirstDecrease(0.1f);
}

void MovableParameters::thirstDecrease(const float & f)
{
	///< @todo : improve with maybe a better function
	m_thirst = fmax(m_thirst - f, 0.0f);
}

float MovableParameters::getDanger() const
{
	return m_danger;
}

void MovableParameters::dangerIncrease()
{
	///< @todo : improve with maybe a better function
	//m_danger = fmin(m_danger + 1.0f, 100.0f);
	m_danger = 100.0f;
}

void MovableParameters::dangerDecrease()
{
	///< @todo : improve with maybe a better function
	//m_danger = fmax(m_danger - 1.0f, 0.0f);
	m_danger = 0.0f;
}

float MovableParameters::getAffinity() const
{
	return m_affinity;
}

void MovableParameters::affinityIncrease()
{
	affinityIncrease(0.1f);
}

void MovableParameters::affinityIncrease(const float & f)
{
	///< @todo : improve with maybe a better function
	m_affinity = fmin(m_affinity + f, 100.0f);
}

void MovableParameters::affinityDecrease()
{
	affinityDecrease(0.1f);
}

void MovableParameters::affinityDecrease(const float & f)
{
	///< @todo : improve with maybe a better function
	m_affinity = fmax(m_affinity - f, 0.0f);
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

bool MovableParameters::isNotHungry() const
{
	return m_hunger >= m_highHungerValue;
}

float MovableParameters::getMaxSpeed() const
{
	return m_maxSpeed;
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
	///< @todo
	return m_danger == 100.0f;
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
	///< @todo
	return m_danger == 0.0f;	
}

float MovableParameters::getDistToLeader() const
{
	return m_distToLeader;
}

float MovableParameters::getDistSeeAhead() const
{
	return m_distSeeAhead;
}