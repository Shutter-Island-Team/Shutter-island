#include "../../include/boids2D/SolverBoid.hpp"

SolverBoid::SolverBoid() {

}

void SolverBoid::solve( const float& dt, std::vector<BoidPtr>& boids ) {
	for(BoidPtr b : boids)
    {
        //TODO: Implement solver
        // b->setVelocity( b->getVelocity() + dt * ( 1.0f/p->getMass() ) * b->getForce() );
        // b->setLocation( b->getLocation() + dt * b->getVelocity() );
    }
}
