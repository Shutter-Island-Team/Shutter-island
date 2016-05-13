#include "./../../include/dynamics/DampingForceField.hpp"

DampingForceField::DampingForceField(const std::vector<ParticlePtr> particles, const float damping)
{
    m_particles = particles;
    m_damping = damping;
}

void DampingForceField::do_addForce()
{
    for(ParticlePtr p : m_particles)
    {
        p->incrForce(-m_damping*p->getVelocity());
    }
}

const std::vector<ParticlePtr> DampingForceField::getParticles()
{
    return m_particles;
}

void DampingForceField::setParticles(const std::vector<ParticlePtr>& particles)
{
    m_particles = particles;
}

const float& DampingForceField::getDamping()
{
    return m_damping;
}

void DampingForceField::setDamping(const float& damping)
{
    m_damping = damping;
}
