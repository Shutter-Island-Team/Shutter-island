#include "../../include/boids2D/SolverBoid.hpp"

SolverBoid::SolverBoid() {

}

void SolverBoid::solve( const float& dt, std::vector<MovableBoidPtr>& boids ) {
	for(MovableBoidPtr b : boids) {
    	b->computeNextStep(dt);
    }
}
