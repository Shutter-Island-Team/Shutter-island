#include "./../../include/dynamics/BoxParticle.hpp"


BoxParticle::BoxParticle(const glm::vec3& position,
			 const glm::vec3& velocity,
			 const float& mass,
			 const float &length,
			 const float &width,
			 const float &height,
			 const ParticlePtr &p1,
			 const ParticlePtr &p2) : 
    StickParticle(position, velocity, mass, p1, p2),
    m_length(length), m_width(width), m_height(height) {
    this->m_isAStick = false;
    this->m_isABox = true;
    // Default : no force should be applied to this stick
    // Position computed "by hand"
    this->setFixed(true);
}

BoxParticle::~BoxParticle()
{}


float BoxParticle::getLength() {
    return m_length;
}

float BoxParticle::getWidth() {
    return m_width;
}

float BoxParticle::getHeight() {
    return m_height;
}
