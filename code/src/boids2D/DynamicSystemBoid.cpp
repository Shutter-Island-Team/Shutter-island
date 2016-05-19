#include "../../include/boids2D/DynamicSystemBoid.hpp"

DynamicSystemBoid::DynamicSystemBoid() : m_dt(0.1) {}

float DynamicSystemBoid::getDt() const
{
    return m_dt;
}

void DynamicSystemBoid::setDt(float dt)
{
    m_dt = dt;
}

SolverBoidPtr DynamicSystemBoid::getSolver()
{
    return m_solver;
}

void DynamicSystemBoid::setSolver(SolverBoidPtr solver)
{
    m_solver = solver;
}

void DynamicSystemBoid::computeSimulationStep()
{
	// TODO : implement it
    //Compute particle's force
    // for(ParticlePtr p : m_particles)
    // {
    //     p->setForce(glm::vec3(0.0,0.0,0.0));
    // }
	// for Boids

    //Integrate position and velocity of particles
    // m_solver->solve(m_dt, m_particles);
	// BoidSolver
}
