#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include "./../../include/gl_helper.hpp"
#include "./../../include/dynamics/DynamicSystem.hpp"

#include "./../../include/dynamics/Particle.hpp"
#include "./../../include/dynamics/RoundedParticle.hpp"

DynamicSystem::DynamicSystem() :
    m_dt(0.1)
{
}


const std::vector<ParticlePtr>& DynamicSystem::getParticles() const
{
    return m_particles;
}

void DynamicSystem::setParticles(const std::vector<ParticlePtr> &particles)
{
    m_particles = particles;
}

const std::vector<ForceFieldPtr>& DynamicSystem::getForceFields() const
{
    return m_forceFields;
}

void DynamicSystem::setForceFields(const std::vector<ForceFieldPtr> &forceFields)
{
    m_forceFields = forceFields;
}


float DynamicSystem::getDt() const
{
    return m_dt;
}

void DynamicSystem::setDt(float dt)
{
    m_dt = dt;
}

DynamicSystem::~DynamicSystem() {}

void DynamicSystem::clear()
{
    m_particles.clear();
    m_forceFields.clear();
}

void DynamicSystem::addParticle(ParticlePtr p)
{
    m_particles.push_back(p);
}

void DynamicSystem::addForceField(ForceFieldPtr forceField)
{
    m_forceFields.push_back(forceField);
}

SolverPtr DynamicSystem::getSolver()
{
    return m_solver;
}

void DynamicSystem::setSolver(SolverPtr solver)
{
    m_solver = solver;
}

void DynamicSystem::computeSimulationStep()
{
    //Compute particle's force
    for(ParticlePtr p : m_particles)
    {
        p->setForce(glm::vec3(0.0,0.0,0.0));
    }
    for(ForceFieldPtr f : m_forceFields)
    {
        f->addForce();
    }

    //Integrate position and velocity of particles
    m_solver->solve(m_dt, m_particles);
}

std::ostream& operator<<(std::ostream& os, const DynamicSystemPtr& system)
{
    std::vector<ParticlePtr> particles = system->getParticles();
    os << "Particle number: " << particles.size() << std::endl;
    for(ParticlePtr p : particles)
        os << p << std::endl;
    return os;
}
