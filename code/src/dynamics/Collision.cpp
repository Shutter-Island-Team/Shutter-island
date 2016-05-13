#include "./../../include/dynamics/Collision.hpp"

Collision::~Collision()
{}

Collision::Collision(float restitution) : m_restitution(restitution)
{}

void Collision::solveCollision()
{
    do_solveCollision();
}
