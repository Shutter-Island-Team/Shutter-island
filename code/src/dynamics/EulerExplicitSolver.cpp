#include "./../../include/dynamics/EulerExplicitSolver.hpp"

EulerExplicitSolver::EulerExplicitSolver()
{

}

EulerExplicitSolver::~EulerExplicitSolver()
{

}

void EulerExplicitSolver::do_solve(const float& dt, std::vector<ParticlePtr>& particles)
{
    for(ParticlePtr p : particles)
    {
        if(!p->isFixed())
        {
            //TODO: Implement explicit euler solver
            //Functions to use:
            //Particle::getPosition(), Particle::getVelocity(), Particle::getMass(), Particle::getForce()
            //Particle::setPosition(), Particle::setVelocity()
            p->setVelocity( p->getVelocity() + dt * ( 1.0f/p->getMass() ) * p->getForce() );
            p->setPosition( p->getPosition() + dt * p->getVelocity() );
        }
    }
}
