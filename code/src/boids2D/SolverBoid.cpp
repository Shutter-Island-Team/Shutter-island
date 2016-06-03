#include "../../include/boids2D/SolverBoid.hpp"
#include "../../include/boids2D/MovableBoid.hpp"

#include <iostream>

SolverBoid::SolverBoid() {

}

void SolverBoid::solve( const float& dt, BoidsManagerPtr boidsManager) {
	glm::vec3 newLocation;
    MatrixMovableBoidPtr mvB = boidsManager->getMovableBoidsMatrix();
    for (int i = 0; i < mvB->getNumLine(); ++i) {
        for (int j = 0; j < mvB->getNumCol(); ++j) {
            for (std::list<MovableBoidPtr>::iterator it = mvB->at(i,j).begin(); it != mvB->at(i,j).end(); ++it) {
                newLocation = (*it)->computeNextStep(dt);
                boidsManager->updateBoid(*it, newLocation, i, j);
            }
        }
    }
}
