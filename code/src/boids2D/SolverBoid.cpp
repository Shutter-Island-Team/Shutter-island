#include "../../include/boids2D/SolverBoid.hpp"

SolverBoid::SolverBoid() {

}

void SolverBoid::solve( const float& dt, std::vector<MovableBoidPtr>& boids ) {
	for(MovableBoidPtr b : boids)
    {
        b->setVelocity( b->getVelocity() + dt * ( 1.0f/b->getMass() ) * b->getAcceleration() );
        b->setLocation( b->getLocation() + dt * b->getVelocity() );
    }
}
