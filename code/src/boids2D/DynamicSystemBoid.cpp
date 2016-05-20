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

void DynamicSystemBoid::setBoidsManager(BoidsManagerPtr boidsManager) {
    m_boidsManager = boidsManager;
}

void DynamicSystemBoid::computeSimulationStep()
{
    for(MovableBoidPtr b : m_boidsManager->getMovableBoids()) {
        b-> computeAcceleration(m_boidsManager->getMovableBoids());
    }

    //Integrate position and velocity of particles
    m_solver->solve(m_dt, m_boidsManager->getMovableBoids());
}

void DynamicSystemBoid::setTargetBoid(float x, float y) {
    for(MovableBoidPtr b : m_boidsManager->getMovableBoids()) {
        b->setTarget(x, y);
    }
}
