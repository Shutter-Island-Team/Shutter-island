#include "../../include/boids2D/BoidsManager.hpp"

BoidsManager::BoidsManager(void) {
	
}

void BoidsManager::addMovableBoid(MovableBoidPtr b) {
	m_movableBoids.push_back(b);
}

std::vector<MovableBoidPtr> & BoidsManager::getMovableBoids() {
	return m_movableBoids;
}