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

/**
 * It is important in this function to note that it first computes
 * the acceleration of each boid then update the boid in two different
 * loops. Therefore each boid can the computation is not polluted by
 * unexpected update of positions of other boids.
 */ 
void DynamicSystemBoid::computeSimulationStep()
{
    std::vector<MovableBoidPtr> mvB = m_boidsManager->getMovableBoids();
    const bool updateTick = m_boidsManager->isUpdateTick();
    // #pragma omp parallel for
    for (unsigned int i = 0; i < mvB.size(); ++i) {
        mvB[i]->computeAcceleration(*m_boidsManager, m_dt, updateTick);
    }

    //Integrate position and velocity of particles
    m_solver->solve(m_dt, m_boidsManager);

    m_boidsManager->removeDead();

    if(updateTick) {
        m_boidsManager->resetTick();
    } else {
        m_boidsManager->updateTick();
    }

    if (m_boidsManager->getCountCarrot() < 20) {
        m_boidsManager->repopCarrot();
    }
}
