#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "../../include/rapidjson/document.h"
#include "../../include/Utils.hpp"
#include "../../include/boids2D/MovableParameters.hpp"

MovableParameters::MovableParameters()
	: MovableParameters(3.5f, 2.0f, 3*M_PI/4, 4.0f, 4.0f, 5.0f)
{

}

MovableParameters::MovableParameters(float maxSpeed, float maxForce,
	float angleView, float distViewSeparate, float distViewCohesion, float distViewMax) : 
	MovableParameters(maxSpeed, maxForce, angleView, distViewSeparate, distViewCohesion, distViewMax,
		5.0f, 9.0f, 3.0f)
{

}

MovableParameters::MovableParameters(float maxSpeed, float maxForce,
	float angleView, float distViewSeparate, float distViewCohesion, float distViewMax,
	float distStartSlowingDown,float rCircleWander, float distToCircle) :
	m_maxSpeed(maxSpeed), m_maxForce(maxForce), m_angleView(angleView),
	m_distViewSeparate(distViewSeparate), m_distViewCohesion(distViewCohesion),
	m_distViewMax(distViewMax),	m_distStartSlowingDown(distStartSlowingDown), m_rCircleWander(rCircleWander),
	m_distToCircle(distToCircle), m_hunger(100.0f), m_thirst(100.0f), m_danger(0.0f),
	m_affinity(0.0f), m_lowStaminaValue(10.0f), m_highStaminaValue(90.0f), m_lowHungerValue(10.0f) 
{
	m_stamina = random(1, 99);
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

	float maxSpeed = d["maxSpeed"].GetDouble();
	float maxForce = d["maxForce"].GetDouble();
	float angleView = d["angleView"].GetDouble();
	float distViewSeparate = d["distViewSeparate"].GetDouble();
	float distViewCohesion = d["distViewCohesion"].GetDouble();
	float distViewMax = d["distViewMax"].GetDouble();
	float distStartSlowingDown = d["distStartSlowingDown"].GetDouble();
	float rCircleWander = d["rCircleWander"].GetDouble();
	float distToCircle = d["distToCircle"].GetDouble();

	*this = MovableParameters(maxSpeed, maxForce, angleView, distViewSeparate, distViewCohesion,
		distViewMax, distStartSlowingDown, rCircleWander, distToCircle);
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

void MovableParameters::associateBoid(MovableBoidPtr thisBoid)
{
	m_thisBoid = thisBoid;
}

float MovableParameters::getStamina() const
{
	return m_stamina;
}

void MovableParameters::staminaIncrease()
{
	return staminaIncrease(0.5f);
}

void MovableParameters::staminaIncrease(const float & f)
{
	m_stamina = fmin(m_stamina + f, 100.0f);
}

void MovableParameters::staminaDecrease()
{
	return staminaDecrease(0.5f);
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
	// TODO : improve with maybe a better function
	m_hunger = fmin(m_hunger + 1.0f, 100.0f);
}

void MovableParameters::hungerDecrease()
{
	// TODO : improve with maybe a better function
	m_hunger = fmax(m_hunger - 1.0f, 0.0f);
}

float MovableParameters::getThirst() const
{
	return m_thirst;
}

void MovableParameters::thirstIncrease()
{
	// TODO : improve with maybe a better function
	m_thirst = fmin(m_thirst + 1.0f, 100.0f);
}

void MovableParameters::thirstDecrease()
{
	// TODO : improve with maybe a better function
	m_thirst = fmax(m_thirst - 1.0f, 0.0f);
}

float MovableParameters::getDanger() const
{
	return m_danger;
}

void MovableParameters::dangerIncrease()
{
	// TODO !!!!!!!!! : improve with a better function
	m_danger = fmin(m_danger + 1.0f, 100.0f);
}

void MovableParameters::dangerDecrease()
{
	// TODO !!!!!!!!! : improve with a better function
	m_danger = fmax(m_danger - 1.0f, 0.0f);
}

float MovableParameters::getAffinity() const
{
	return m_affinity;
}

void MovableParameters::affinityIncrease()
{
	// TODO : improve with maybe a better function
	m_affinity = fmin(m_affinity + 1.0f, 100.0f);
}

void MovableParameters::affinityDecrease()
{
	// TODO : improve with maybe a better function
	m_affinity = fmax(m_affinity - 1.0f, 0.0f);
}

bool MovableParameters::isLeader() const
{
	return m_leader == m_thisBoid;
}

MovableBoidPtr MovableParameters::getLeader() const
{
	return m_leader;
}

void MovableParameters::setNewLeader(MovableBoidPtr newLeader)
{
	m_leader = newLeader;
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
	return m_distViewSeparate;
}

float MovableParameters::getDistViewCohesion() const
{
	return m_distViewCohesion;
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
	// TODO : A faire
	return false;
}

bool MovableParameters::isThursty() const
{
	// TODO : A faire
	return false;
}

bool MovableParameters::isNotThursty() const
{
	// TODO : A faire
	return false;	
}

bool MovableParameters::isNotTired() const
{
	// TODO : A faire
	return false;	
}

bool MovableParameters::isNotInDanger() const
{
	// TODO : A faire
	return false;	
}
