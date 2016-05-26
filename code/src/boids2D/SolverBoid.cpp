#include "../../include/boids2D/SolverBoid.hpp"
#include "../../include/boids2D/MovableBoid.hpp"

#include <iostream>

SolverBoid::SolverBoid() {

}

void SolverBoid::solve( const float& dt, BoidsManagerPtr boidsManager) {
	for(MovableBoidPtr b : boidsManager->getMovableBoids()) {
    	b->computeNextStep(dt);
    }
}
