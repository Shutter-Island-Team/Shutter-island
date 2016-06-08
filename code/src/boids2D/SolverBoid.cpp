#include "../../include/boids2D/SolverBoid.hpp"
#include "../../include/boids2D/MovableBoid.hpp"

#include <iostream>

SolverBoid::SolverBoid() {

}

void SolverBoid::solve( const float& dt, BoidsManagerPtr boidsManager) {
    glm::vec3 prevLocation;
    unsigned int iprev;
    unsigned int jprev;
    std::vector<MovableBoidPtr> mvB = boidsManager->getMovableBoids();
    #pragma omp parallel for private(prevLocation, iprev, jprev)
    for (unsigned int i = 0; i < mvB.size(); ++i) {
        prevLocation = mvB[i]->computeNextStep(dt, boidsManager);
        boidsManager->coordToBox(prevLocation, iprev, jprev);
        boidsManager->updateBoidInGrid(mvB[i], iprev, jprev);
    }
}
