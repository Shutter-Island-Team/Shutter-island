#include "../../include/boids2D/MovableParameters.hpp"
#include <cmath>
#include <iostream>
#include <fstream>
#include "../../include/Utils.hpp"

MovableParameters::MovableParameters()
	: MovableParameters(3.5f, 2.0f, 3*M_PI/4, 2.0f, 4.0f, 5.0f)
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
	// file >> m_maxSpeed;
	// file >> m_maxForce;
	// file >> m_angleView;
	// file >> m_distViewSeparate;
	// file >> m_distViewCohesion;
	// file >> m_distStartSlowingDown;
	// file >> m_rCircleWander;
	// file >> m_distToCircle;
}

MovableParameters::MovableParameters(BoidType type)
{
	switch (type) {
		case RABBIT:
			std::cerr << "Implementation rabbit" << std::endl;
			*this = MovableParameters( "../../XML_data/RabbitParameters.xml" ); 
			break;
		case WOLF:
			std::cerr << "Implementation wolf" << std::endl;
			*this = MovableParameters( "../../XML_data/WolfParameters.xml" );
			break;
		default:
			std::cerr << "Unknown animal" << std::endl;
			break;
	}
}

void MovableParameters::initializeParameters(MovableBoidPtr thisBoid)
{
	m_thisBoid = thisBoid;
}

float MovableParameters::getStamina()
{
	return m_stamina;
}

void MovableParameters::staminaIncrease()
{
	return staminaIncrease(0.5f);
}

void MovableParameters::staminaIncrease(float f)
{
	m_stamina = fmin(m_stamina + f, 100.0f);
}

void MovableParameters::staminaDecrease()
{
	return staminaDecrease(0.5f);
}

void MovableParameters::staminaDecrease(float f)
{
	m_stamina = fmax(m_stamina - f, 0.0f);
}

float MovableParameters::getHunger()
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

float MovableParameters::getThirst()
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

float MovableParameters::getDanger()
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

float MovableParameters::getAffinity()
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

bool MovableParameters::isLeader()
{
	return m_leader == m_thisBoid;
}

MovableBoidPtr MovableParameters::getLeader()
{
	return m_leader;
}

void MovableParameters::setNewLeader(MovableBoidPtr newLeader)
{
	m_leader = newLeader;
}

bool MovableParameters::isLowStamina() {
	return m_stamina <= m_lowStaminaValue;
}

bool MovableParameters::isHighStamina() {
	return m_stamina >= m_highStaminaValue;
}

bool MovableParameters::isLowHunger() {
	return m_hunger <= m_lowHungerValue;
}

float MovableParameters::getMaxSpeed() {
	return m_maxSpeed;
}
float MovableParameters::getMaxForce() {
	return m_maxForce;
}
float MovableParameters::getRadiusCircleWander() {
	return m_rCircleWander;
}
float MovableParameters::getDistToCircleWander() {
	return m_distToCircle;
}
float MovableParameters::getDistStartSlowingDown() {
	return m_distStartSlowingDown;
}

float MovableParameters::getDistSeparate() {
	return m_distViewSeparate;
}

float MovableParameters::getDistViewCohesion() {
	return m_distViewCohesion;
}

float MovableParameters::getAngleView() {
	return m_angleView;
}

float MovableParameters::getDistViewMax() {
	return m_distViewMax;
}

