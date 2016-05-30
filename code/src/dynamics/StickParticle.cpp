#include "./../../include/dynamics/StickParticle.hpp"


StickParticle::StickParticle(const glm::vec3 &position, 
			     const glm::vec3 &velocity, 
			     const float &mass, 
			     const ParticlePtr &p1,
			     const ParticlePtr &p2)
  : Particle(position, velocity, mass),
    m_p1(p1), m_p2(p2) {
    this->m_isAStick = true;
    // Default : can be affected by its hierarchy
    this->m_isDragged = true;
    // Default : no force should be applied to this stick
    // Position computed "by hand"
    this->setFixed(true);
    // Relative positions
    m_p1_pos = glm::vec3(m_p1->getPosition() - this->getPosition());
    m_p2_pos = glm::vec3(m_p2->getPosition() - this->getPosition());
}

StickParticle::~StickParticle()
{}



glm::vec3 StickParticle::getOrientation() const
{
    return m_orientation;
}


void StickParticle::updateHierarchy() {
    // Handling the collision
    if (m_collisionToHandle) {
	m_collisionToHandle = false;
	this->handleCollision(m_penetration, m_collisionDirection);
    }
    // Eventually calling recursively this function
    if (m_p1->isAStick() || m_p1->isABox()) {
	StickParticlePtr stickPtr1 = (StickParticlePtr) std::static_pointer_cast<StickParticle, Particle>(m_p1);
	stickPtr1->updateHierarchy();
    }
    if (m_p2->isAStick() || m_p2->isABox()) {
	StickParticlePtr stickPtr2 = (StickParticlePtr) std::static_pointer_cast<StickParticle, Particle>(m_p2);
	stickPtr2->updateHierarchy();
    }
    
    // Recomputing the velocity according to the children configuration
    if (m_controlChild1) {
	if (m_controlChild2) {
	    glm::vec3 position = this->getPosition(); 
	    glm::vec3 velocity = this->getVelocity(); 
	    m_p1->setPosition(position + m_p1_pos);
	    m_p2->setPosition(position + m_p2_pos);
	    m_p1->setVelocity(velocity);
	    m_p2->setVelocity(velocity);
	} else {
	    this->setPosition(m_p2->getPosition() - m_p2_pos);
	    m_p1->setPosition(this->getPosition() + m_p1_pos);
	    this->setVelocity(m_p2->getVelocity());
 	    m_p1->setVelocity(this->getVelocity());
	}
    } else {
	if (m_controlChild2) {
	    
	    glm::vec3 p = this->getPosition();
	    this->setPosition(m_p1->getPosition() - m_p1_pos);
	    m_p2->setPosition(this->getPosition() + m_p2_pos);
	    this->setVelocity(m_p1->getVelocity());
   	    m_p2->setVelocity(this->getVelocity());
	} else {
	    this->setPosition((m_p1->getPosition() + m_p2->getPosition())*0.5f);
	    this->setVelocity((m_p1->getVelocity() + m_p2->getVelocity())*0.5f);
	}
    }
}



void StickParticle::setControlChild1(bool control) {
    this->m_controlChild1 = control;
}


void StickParticle::setControlChild2(bool control) {
    this->m_controlChild2 = control;
}


void StickParticle::setControlChildren(bool control) {
    this->setControlChild1(control);
    this->setControlChild2(control);
}

void StickParticle::setCollision(float penetration, glm::vec3 collisionDirection) {
    m_collision++;
    this->m_collisionToHandle = true;
    this->m_penetration = penetration;
    // Correcting the direction of the collision
    if ((glm::dot(collisionDirection, this->getVelocity())) > 0) 
	this->m_collisionDirection = glm::vec3(collisionDirection);
    else 
	this->m_collisionDirection = glm::vec3(collisionDirection) * -1.0f;
}

// 3 is left as a a security
// if the velocity stays low, the effect of the 3. will not be visible
void StickParticle::handleCollision(float penetration, glm::vec3 collisionDirection) {;
    
    // Correcting its position
    this->setPosition(this->getPosition() + collisionDirection * (-3.f*penetration));

    // Correcting its velocity
    glm::vec3 oldVelocity = this->getVelocity();
    glm::vec3 collisionVelocity = collisionDirection * glm::dot(oldVelocity, collisionDirection);
    this->setVelocity(oldVelocity - (collisionVelocity * 2.f));
    
    // First child
    if (m_p1->isAStick() || m_p1->isABox()) {
	StickParticlePtr stickPtr1 = (StickParticlePtr) std::static_pointer_cast<StickParticle, Particle>(m_p1);
	stickPtr1->handleCollision(penetration, collisionDirection);
    } else {
	m_p1->setPosition(m_p1->getPosition() + collisionDirection * (-3.f*penetration));
	oldVelocity = m_p1->getVelocity();
	collisionVelocity = collisionDirection * glm::dot(oldVelocity, collisionDirection);
	m_p1->setVelocity(oldVelocity - (collisionVelocity * 2.f));
    }
    // Second child
    if (m_p2->isAStick() || m_p2->isABox()) {
	StickParticlePtr stickPtr2 = (StickParticlePtr) std::static_pointer_cast<StickParticle, Particle>(m_p2);
	stickPtr2->handleCollision(penetration, collisionDirection);
    } else {
	m_p2->setPosition(m_p2->getPosition() + collisionDirection * (-3.f*penetration));
	oldVelocity = m_p2->getVelocity();
	collisionVelocity = collisionDirection * glm::dot(oldVelocity, collisionDirection);
	m_p2->setVelocity(oldVelocity - (collisionVelocity * 2.f));
    }
}
