#include "../../include/boids2D/DynamicSystemBoid.hpp"

#include <iostream>

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

void DynamicSystemBoid::setBoidsManager(BoidsManagerPtr boidsManager) {
    m_boidsManager = boidsManager;
}

#include <omp.h>

/**
 * It is important in this function to note that it first computes
 * the acceleration of each boid then update the boid in two different
 * loops. Therefore each boid can the computation is not polluted by
 * unexpected update of positions of other boids.
 */ 
void DynamicSystemBoid::computeSimulationStep()
{
    MatrixMovableBoidPtr mvB = m_boidsManager->getMovableBoidsMatrix();

    for (int i = 0; i < mvB->getNumLine(); ++i) {
        for (int j = 0; j < mvB->getNumCol(); ++j) {
            for (std::list<MovableBoidPtr>::iterator it = mvB->at(i,j).begin(); it != mvB->at(i,j).end(); ++it) {
                (*it)->computeAcceleration(*m_boidsManager, m_dt, i, j);
            }
        }
    }

    //Integrate position and velocity of particles
    m_solver->solve(m_dt, m_boidsManager);

    m_boidsManager->removeDead();
}
