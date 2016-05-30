#include "./../../include/dynamics/Particle.hpp"

Particle::Particle(const glm::vec3 &position, 
                 const glm::vec3 &velocity, 
                 const float &mass)
    : m_initialPosition( position ),
      m_position(position),
      m_initialVelocity( velocity ),
      m_velocity(velocity),
      m_force(glm::vec3(0.0,0.0,0.0)),
      m_mass(mass)
{}

bool Particle::isFixed() const
{
    return m_isFixed;
}

void Particle::setFixed(bool isFixed)
{
    m_isFixed = isFixed;
}

bool Particle::isCollisionable() const
{
    return m_isCollisionable;
}

void Particle::setCollisionable(bool isCollisionable)
{
    m_isCollisionable = isCollisionable;
}


bool Particle::isDragged() const
{
    return m_isDragged;
}

void Particle::setDragged(bool isDragged)
{
    m_isDragged = isDragged;
}

Particle::~Particle()
{}

const glm::vec3 & Particle::getPosition() const
{
    return m_position;
}

const glm::vec3 & Particle::getVelocity() const
{
    return m_velocity;
}

const glm::vec3 & Particle::getForce() const
{
    return m_force;
}

float Particle::getMass() const
{
    return m_mass;
}


void Particle::setPosition(const glm::vec3 &pos)
{
    m_position = pos;
}

void Particle::setVelocity(const glm::vec3 &vel)
{	
    m_velocity = vel;
}

void Particle::setForce(const glm::vec3 &force)
{
    m_force = force;
}

void Particle::incrPosition(const glm::vec3 &pos)
{
    m_position += pos;
}

void Particle::incrVelocity(const glm::vec3 &vel)
{
    m_velocity += vel;
}

void Particle::incrForce(const glm::vec3& force)
{
    m_force +=force;
}

void Particle::restart()
{
  m_position = m_initialPosition;
  m_velocity = m_initialVelocity;
}


void Particle::setIsFollowedCamera(bool val) {
    m_isFollowed = val;
}

void Particle::recomputeCamera() {
    if (m_isFollowed) {
	glm::vec3 position =  glm::vec3(this->getPosition());
	glm::vec3 velocity =  glm::vec3(this->getVelocity());
	if (!m_initCamera) { 
	    this->m_oldPosition = position;
	} else {
	    this->m_oldPosition = (this->m_oldPosition * 0.5f) + (position * 0.5f);
	}
	if (glm::length(velocity) > 0.1) {
	    if (!m_initCamera) { 
		  m_initCamera = true;
		  this->m_cameraDirection = glm::normalize(velocity);
	    } else {	
		  this->m_cameraDirection = (this->m_cameraDirection * 0.99f) + (glm::normalize(velocity) * 0.01f);
	    }
    }

	float scale_recoil = -20.0;//-10.0;
	float z_offset = 10.0;
	this->m_cameraPosition = m_oldPosition + m_cameraDirection*scale_recoil + glm::vec3(0.0, 0.0, z_offset);
	this->m_cameraTarget = glm::vec3(m_oldPosition) + glm::vec3(0.0, 0.0, 1.*z_offset/2.0);
    }
}

glm::vec3 Particle::getCameraPosition() {
    return this->m_cameraPosition;
}
glm::vec3 Particle::getCameraTarget() {
    return this->m_cameraTarget;
}


bool Particle::isRounded() {
    return m_isRounded;
}

bool Particle::isAStick() {
    return m_isAStick;
}

bool Particle::isABox() {
    return m_isABox;
}


std::ostream& operator<<(std::ostream& os, const ParticlePtr& p)
{
    const glm::vec3& x = p->getPosition();
    const glm::vec3& v = p->getVelocity();

    os << "pos (" << x[0] << ", " << x[1] << ", " << x[2] << ")";
    os << " ; ";
    os << "vel (" << v[0] << ", " << v[1] << ", " << v[2] << ")";

    return os;
}

