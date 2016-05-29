#include "./../../include/dynamics/ParticlePlaneCollision.hpp"
#include <cmath>

ParticlePlaneCollision::~ParticlePlaneCollision()
{}


ParticlePlaneCollision::ParticlePlaneCollision(ParticlePtr particle, PlanePtr plane, float restitution) :
    Collision(restitution)
{
    m_particle = particle;
    m_plane = plane;
}

void ParticlePlaneCollision::do_solveCollision()
{
    //Don't process fixed particles (Let's assume that the ground plane is fixed)
    if (m_particle->isFixed() && (!(m_particle->isDragged()))) return;

    //TODO: Solve ParticlePlane collisions, update particle position and velocity after collision
    //Functions to use:
    //glm::dot(v1, v2): Return the dot product of two vector.
    //Plane::distanceToOrigin(): Return the distance to origin from the plane
    //Plane::normal(): Return the normal of the plane
    //Particle::getRadius(), Particle::getPosition(), Particle::getVelocity(), Particle::setPosition(), Particle::setVelocity()
    if (m_particle->isRounded()) {
	RoundedParticlePtr ballPtr = (RoundedParticlePtr) std::static_pointer_cast<RoundedParticle, Particle>(m_particle);
	//Compute particle-plane distance
	glm::vec3 prev_x = ballPtr->getPosition();
	float distToPlane = glm::dot(prev_x, m_plane->normal()) - m_plane->distanceToOrigin();
	//Project the particle on the plane
	glm::vec3 new_x = prev_x - (distToPlane - ballPtr->getRadius())*m_plane->normal();
	ballPtr->setPosition(new_x);

	//Compute post-collision velocity
	glm::vec3 prev_v = ballPtr->getVelocity();
	glm::vec3 new_v = prev_v - (1.0f + m_restitution)*glm::dot(prev_v, m_plane->normal())*m_plane->normal();
	ballPtr->setVelocity(new_v);
    }

    //     7----------8                    ^
    //    /|         /|                    |  height
    //  5--------- 6  |
    //  |  |   c   |  |                width
    //  |  3-------|- 4                  /
    //  | /        | /      
    //  1----------2                   -->length
    if (m_particle->isABox()) {
	    
	BoxParticlePtr boxPtr = (BoxParticlePtr) std::static_pointer_cast<BoxParticle, Particle>(m_particle);

	// Real case : rotation should be applied to the glm::vec3 below
	glm::vec3 col1 = glm::vec3(-(boxPtr->getLength())/2, -(boxPtr->getWidth())/2, -(boxPtr->getHeight())/2);
	glm::vec3 col2 = glm::vec3( (boxPtr->getLength())/2, -(boxPtr->getWidth())/2, -(boxPtr->getHeight())/2);
	glm::vec3 col3 = glm::vec3(-(boxPtr->getLength())/2,  (boxPtr->getWidth())/2, -(boxPtr->getHeight())/2);
	glm::vec3 col4 = glm::vec3( (boxPtr->getLength())/2,  (boxPtr->getWidth())/2, -(boxPtr->getHeight())/2);
	glm::vec3 col5 = glm::vec3(-(boxPtr->getLength())/2, -(boxPtr->getWidth())/2,  (boxPtr->getHeight())/2);
	glm::vec3 col6 = glm::vec3( (boxPtr->getLength())/2, -(boxPtr->getWidth())/2,  (boxPtr->getHeight())/2);
	glm::vec3 col7 = glm::vec3(-(boxPtr->getLength())/2,  (boxPtr->getWidth())/2,  (boxPtr->getHeight())/2);
	glm::vec3 col8 = glm::vec3( (boxPtr->getLength())/2,  (boxPtr->getWidth())/2,  (boxPtr->getHeight())/2);

	glm::vec3 p1 = glm::vec3(boxPtr->getPosition() + col1);
	glm::vec3 p2 = glm::vec3(boxPtr->getPosition() + col2);
	glm::vec3 p3 = glm::vec3(boxPtr->getPosition() + col3);
	glm::vec3 p4 = glm::vec3(boxPtr->getPosition() + col4);
	glm::vec3 p5 = glm::vec3(boxPtr->getPosition() + col5);
	glm::vec3 p6 = glm::vec3(boxPtr->getPosition() + col6);
	glm::vec3 p7 = glm::vec3(boxPtr->getPosition() + col7);
	glm::vec3 p8 = glm::vec3(boxPtr->getPosition() + col8);

	// Penetration factor
	float pen1 = glm::dot(p1, m_plane->normal()) - m_plane->distanceToOrigin();
	float penetration = std::abs(pen1);
	float pen2 = glm::dot(p2, m_plane->normal()) - m_plane->distanceToOrigin();
	penetration = std::min(penetration, std::abs(pen2));
	float pen3 = glm::dot(p3, m_plane->normal()) - m_plane->distanceToOrigin();
	penetration = std::min(penetration, std::abs(pen3));
	float pen4 = glm::dot(p4, m_plane->normal()) - m_plane->distanceToOrigin();
	penetration = std::min(penetration, std::abs(pen4));
	float pen5 = glm::dot(p5, m_plane->normal()) - m_plane->distanceToOrigin();
	penetration = std::min(penetration, std::abs(pen5));
	float pen6 = glm::dot(p6, m_plane->normal()) - m_plane->distanceToOrigin();
	penetration = std::min(penetration, std::abs(pen6));
	float pen7 = glm::dot(p7, m_plane->normal()) - m_plane->distanceToOrigin();
	penetration = std::min(penetration, std::abs(pen7));
	float pen8 = glm::dot(p8, m_plane->normal()) - m_plane->distanceToOrigin();
	penetration = std::min(penetration, std::abs(pen8));


	
	// Computing the collision direction
	glm::vec3 collisionDirection = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 zero = glm::vec3(0.0, 0.0, 0.0);
	collisionDirection += (pen1>0)?(col1):(zero);
	collisionDirection += (pen2>0)?(col2):(zero);
	collisionDirection += (pen3>0)?(col3):(zero);
	collisionDirection += (pen4>0)?(col4):(zero);
	collisionDirection += (pen5>0)?(col5):(zero);
	collisionDirection += (pen6>0)?(col6):(zero);
	collisionDirection += (pen7>0)?(col7):(zero);
	collisionDirection += (pen8>0)?(col8):(zero);
	// As we don't know how is the collision ie which vertices has gone through the plane
	// the collision direction must be modified according to the velocity
	collisionDirection = glm::normalize(collisionDirection);
	boxPtr->setCollision(penetration, collisionDirection); 
    }
}



bool testParticlePlane(const ParticlePtr &particle, const PlanePtr &plane)
{
    /* Equation of a plane passing through A and normal to n:
   * dot( p - A, n ) = dot( p, n ) - dot( A, n ) = 0
   * dot( A, n ) is stored in the "distanceToOrigin" of the plane.
   *
   * Equation of a particle of radius r centered in c:
   * dot( p - c, p - c ) = r²
   *
   * distance( plane, particle )
   *   = min( distance( plane, c ) - r, 0 )              //definition
   *   = min( abs( dot( c - A, n ) ) - r, 0 )
   *   = min( abs( dot( c, n ) - dot( A, n ) ) - r, 0 )
   *
   * So, there is intersection if distance( plane, particle ) = 0
   * <=> abs( dot( c, n ) - dot( A, n ) ) - r <= 0
   * <=> abs( dot( c, n ) - dot( A, n ) ) <= r
   */

    //TODO: Test collision between particle and plane
    //Functions to use:
    //glm::dot(v1, v2): Return the dot product of two vector.
    //Plane::distanceToOrigin(): Return the distance to origin from the plane
    //Plane::normal(): Return the normal of the plane
    //Particle::getRadius(), Particle::getPosition()

    //return false;
    
    if (particle->isRounded()) {
	RoundedParticlePtr ballPtr = (RoundedParticlePtr) std::static_pointer_cast<RoundedParticle, Particle>(particle);
	return std::abs(dot(ballPtr->getPosition(), plane->normal())-plane->distanceToOrigin() )
            <= ballPtr->getRadius();
    }

    //     7----------8                    ^
    //    /|         /|                    |  height
    //  5--------- 6  |
    //  |  |   c   |  |                width
    //  |  3-------|- 4                  /
    //  | /        | /      
    //  1----------2                   -->length
    if (particle->isABox()) {
	// Real case : rotation should be applied to the glm::vec3 below
	BoxParticlePtr boxPtr = (BoxParticlePtr) std::static_pointer_cast<BoxParticle, Particle>(particle);
	glm::vec3 p1 = glm::vec3(boxPtr->getPosition() + glm::vec3(-(boxPtr->getLength())/2, -(boxPtr->getWidth())/2, -(boxPtr->getHeight())/2));
	glm::vec3 p2 = glm::vec3(boxPtr->getPosition() + glm::vec3( (boxPtr->getLength())/2, -(boxPtr->getWidth())/2, -(boxPtr->getHeight())/2));
	glm::vec3 p3 = glm::vec3(boxPtr->getPosition() + glm::vec3(-(boxPtr->getLength())/2,  (boxPtr->getWidth())/2, -(boxPtr->getHeight())/2));
	glm::vec3 p4 = glm::vec3(boxPtr->getPosition() + glm::vec3( (boxPtr->getLength())/2,  (boxPtr->getWidth())/2, -(boxPtr->getHeight())/2));
	glm::vec3 p5 = glm::vec3(boxPtr->getPosition() + glm::vec3(-(boxPtr->getLength())/2, -(boxPtr->getWidth())/2,  (boxPtr->getHeight())/2));
	glm::vec3 p6 = glm::vec3(boxPtr->getPosition() + glm::vec3( (boxPtr->getLength())/2, -(boxPtr->getWidth())/2,  (boxPtr->getHeight())/2));
	glm::vec3 p7 = glm::vec3(boxPtr->getPosition() + glm::vec3(-(boxPtr->getLength())/2,  (boxPtr->getWidth())/2,  (boxPtr->getHeight())/2));
	glm::vec3 p8 = glm::vec3(boxPtr->getPosition() + glm::vec3( (boxPtr->getLength())/2,  (boxPtr->getWidth())/2,  (boxPtr->getHeight())/2));
	
	// There is a collision iff two points of the box have opposite sign when testing the plane equation
	int pos = 0, neg = 0;
	((glm::dot(p1, plane->normal()) - plane->distanceToOrigin()) > 0)?(pos++):(neg++);
	((glm::dot(p2, plane->normal()) - plane->distanceToOrigin()) > 0)?(pos++):(neg++);
	((glm::dot(p3, plane->normal()) - plane->distanceToOrigin()) > 0)?(pos++):(neg++);
	((glm::dot(p4, plane->normal()) - plane->distanceToOrigin()) > 0)?(pos++):(neg++);
	((glm::dot(p5, plane->normal()) - plane->distanceToOrigin()) > 0)?(pos++):(neg++);
	((glm::dot(p6, plane->normal()) - plane->distanceToOrigin()) > 0)?(pos++):(neg++);
	((glm::dot(p7, plane->normal()) - plane->distanceToOrigin()) > 0)?(pos++):(neg++);
	((glm::dot(p8, plane->normal()) - plane->distanceToOrigin()) > 0)?(pos++):(neg++);
	return ((pos > 0) && (neg > 0));
    }
    return false;
}
