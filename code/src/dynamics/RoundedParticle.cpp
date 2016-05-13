#include "./../../include/dynamics/RoundedParticle.hpp"


RoundedParticle::RoundedParticle(const glm::vec3 &position, 
				 const glm::vec3 &velocity, 
				 const float &mass, 
				 const float &radius)
    : Particle(position, velocity, mass),
      m_radius(radius) {
    this->m_isRounded = true;
}

RoundedParticle::~RoundedParticle()
{}



void RoundedParticle::setRadius(const float &radius)
{
    m_radius = radius;
}


float RoundedParticle::getRadius() const
{
    return m_radius;
}


