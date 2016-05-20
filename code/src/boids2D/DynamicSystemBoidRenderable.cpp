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

void DynamicSystemBoidRenderable::do_mousePressEvent(sf::Event& e) {
	choice = (choice + 1) % 3;
    switch(choice) {
        case 0:
            m_system->setTargetBoid(0, 0);
            break;
        case 1:
            m_system->setTargetBoid(5, 5);
            break;
        case 2:
            m_system->setTargetBoid(10,10);
            break;
        default:
            std::cerr << "Unexpected Target" << std::endl;
            m_system->setTargetBoid(0, 0);            
            break;
    }
}

