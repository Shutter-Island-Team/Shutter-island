#include "../../include/boids2D/BoidsManager.hpp"

BoidsManager::BoidsManager(void) {
	
}

void BoidsManager::add(MovableBoid b) {
	m_movableBoids.push_back(b);
}

void BoidsManager::moveAllBoids() {
	 for(size_t i=0; i<m_movableBoids.size(); ++i) {
	 	MovableBoid b = m_movableBoids[i];
	 	if(b.isMovable()) {
	 		b.move(m_movableBoids);
	 	}
	 }
}