#include "./../../include/dynamics/ConstantForceField.hpp"

ConstantForceField::ConstantForceField(const std::vector<ParticlePtr>& particles, const glm::vec3& force)
{
    m_particles = particles;
    m_force = force;
}

void ConstantForceField::do_addForce()
{
    for(ParticlePtr p : m_particles)
    {
        p->incrForce(m_force*p->getMass());
    }
}

const std::vector<ParticlePtr> ConstantForceField::getParticles()
{
    return m_particles;
}

void ConstantForceField::setParticles(const std::vector<ParticlePtr>& particles)
{
    m_particles = particles;
}

const glm::vec3& ConstantForceField::getForce()
{
    return m_force;
}

void ConstantForceField::setForce(const glm::vec3& force)
{
    m_force = force;
}
