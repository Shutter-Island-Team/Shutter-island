#include "../../include/boids2D/MovableParameters.hpp"
#include <cmath>
#include <iostream>

MovableParameters::MovableParameters(MovableBoidPtr leader)
	: m_stamina(100.0f), m_hunger(100.0f), m_thirst(100.0f), m_danger(0.0f),
	m_affinity(0.0f), m_leader(leader), m_lowStaminaValue(10.0f), m_highStaminaValue(99.0f)
{

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
	// TODO : improve with maybe a better function
	m_stamina = fmin(m_stamina + 1.0f, 100.0f);
}

void MovableParameters::staminaDecrease()
{
	// TODO : improve with maybe a better function
	m_stamina = fmax(m_stamina - 1.0f, 0.0f);
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

