#include "../../include/boids2D/BoidsManager.hpp"

BoidsManager::BoidsManager(void) {
	
}

void BoidsManager::addMovableBoid(MovableBoidPtr b) {
	m_movableBoids.push_back(b);
}

void BoidsManager::addRootedBoid(RootedBoidPtr r)
{
	m_rootedBoids.push_back(r);
}

std::vector<MovableBoidPtr>& BoidsManager::getMovableBoids() {
	return m_movableBoids;
}

std::vector<RootedBoidPtr>& BoidsManager::getRootedBoids() {
	return m_rootedBoids;
}
