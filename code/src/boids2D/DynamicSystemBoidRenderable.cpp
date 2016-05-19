#include "../../include/boids2D/DynamicSystemBoidRenderable.hpp"

DynamicSystemBoidRenderable::DynamicSystemBoidRenderable(DynamicSystemBoidPtr system) :
    HierarchicalRenderable(nullptr), m_lastUpdateTime( 0 )
{
    m_system = system;
}

void DynamicSystemBoidRenderable::do_draw()
{}

void DynamicSystemBoidRenderable::do_animate(float time )
{
    if( time - m_lastUpdateTime >= m_system->getDt() )
    {
        //Dynamic system step
        m_system->computeSimulationStep();
        m_lastUpdateTime = time;
    }

}

void DynamicSystemBoidRenderable::setDynamicSystem(const DynamicSystemBoidPtr &system)
{
    m_system = system;
}
