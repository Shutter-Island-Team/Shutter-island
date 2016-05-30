#include <string>
#include <fstream>
#include <streambuf>
#include "../../include/boids2D/ForceController.hpp"
#include "../../include/rapidjson/document.h"

ForceController::ForceController()
{
	rapidjson::Document d;

	std::ifstream t("../boidData/ForcesParameters.json");
	std::string str;

	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
	            std::istreambuf_iterator<char>());	

	d.Parse(str.c_str());

	m_separate = d["separate"].GetDouble();
	m_evade = d["evade"].GetDouble();
	m_cohesion = d["cohesion"].GetDouble();
	m_align = d["align"].GetDouble();
	m_stayWithinWalls = d["stayWithinWalls"].GetDouble();
	m_collisionAvoidance = d["collisionAvoidance"].GetDouble();
	m_followLeader = d["followLeader"].GetDouble();
}

float ForceController::getSeparate() const 
{
	return m_separate;
}

float ForceController::getEvade() const 
{
	return m_evade;
}

float ForceController::getCohesion() const 
{
	return m_cohesion;
}

float ForceController::getAlign() const 
{
	return m_align;
}

float ForceController::getStayWithinWalls() const 
{
	return m_stayWithinWalls;
}

float ForceController::getCollisionAvoidance() const 
{
	return m_collisionAvoidance;
}

float ForceController::getFollowLeader() const
{
	return m_followLeader;
}
