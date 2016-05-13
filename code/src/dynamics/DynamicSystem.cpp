#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include "./../../include/gl_helper.hpp"
#include "./../../include/dynamics/DynamicSystem.hpp"
#include "./../../include/dynamics/ParticlePlaneCollision.hpp"
#include "./../../include/dynamics/ParticleParticleCollision.hpp"

#include "./../../include/dynamics/Particle.hpp"
#include "./../../include/dynamics/RoundedParticle.hpp"

DynamicSystem::DynamicSystem() :
    m_dt(0.1),
    m_restitution(1.0),
    m_handleCollisions(true)
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
    m_planeObstacles.clear();
}

bool DynamicSystem::getCollisionDetection()
{
    return m_handleCollisions;
}

void DynamicSystem::setCollisionsDetection(bool onOff)
{
    m_handleCollisions = onOff;
}

void DynamicSystem::addParticle(ParticlePtr p)
{
    m_particles.push_back(p);
}

void DynamicSystem::addForceField(ForceFieldPtr forceField)
{
    m_forceFields.push_back(forceField);
}

void DynamicSystem::addPlaneObstacle(PlanePtr planeObstacle)
{
    m_planeObstacles.push_back(planeObstacle);
}

SolverPtr DynamicSystem::getSolver()
{
    return m_solver;
}

void DynamicSystem::setSolver(SolverPtr solver)
{
    m_solver = solver;
}

void DynamicSystem::detectCollisions()
{
    //Detect particle plane collisions
    for(ParticlePtr p : m_particles)
	{
	    if (p->isCollisionable()) {
		for(PlanePtr o : m_planeObstacles)
		    {
			if(testParticlePlane(p, o))
			    {
				ParticlePlaneCollisionPtr c = std::make_shared<ParticlePlaneCollision>(p, o, m_restitution);
				m_collisions.push_back(c);
			    }
		    }
	    }
	}
    //Detect particle particle collisions
    for(size_t i=0; i<m_particles.size(); ++i)
	{
	    for(size_t j=i; j<m_particles.size(); ++j)
		{
		    ParticlePtr p1 = m_particles[i];
		    ParticlePtr p2 = m_particles[j];
		    if (p1->isCollisionable() and p2->isCollisionable()) {
			if(testParticleParticle(p1, p2))
			    {
				ParticleParticleCollisionPtr c = std::make_shared<ParticleParticleCollision>(p1, p2, m_restitution);
				m_collisions.push_back(c);
			    }
		    }
		}
	}
}

void DynamicSystem::solveCollisions()
{
    while(!m_collisions.empty())
    {
        CollisionPtr collision = m_collisions.back();
	collision->solveCollision();
        m_collisions.pop_back();
    }
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

    //Detect and resolve collisions
    if(m_handleCollisions)
    {
        detectCollisions();
        solveCollisions();
    }
}

const float DynamicSystem::getRestitution()
{
    return m_restitution;
}

void DynamicSystem::setRestitution(const float& restitution)
{
    m_restitution = std::max(0.0f,std::min(restitution,1.0f));
}

std::ostream& operator<<(std::ostream& os, const DynamicSystemPtr& system)
{
    std::vector<ParticlePtr> particles = system->getParticles();
    os << "Particle number: " << particles.size() << std::endl;
    for(ParticlePtr p : particles)
        os << p << std::endl;
    return os;
}
