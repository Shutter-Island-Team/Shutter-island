#include "./../../include/dynamics/ParticleParticleCollision.hpp"
#include <glm/gtx/norm.hpp>

ParticleParticleCollision::~ParticleParticleCollision()
{

}

ParticleParticleCollision::ParticleParticleCollision(ParticlePtr particle1, ParticlePtr particle2, float restitution) :
    Collision(restitution)
{
    m_p1 = particle1;
    m_p2 = particle2;
}

void ParticleParticleCollision::do_solveCollision()
{
    //Don't process fixed particles (Let's assume that the ground plane is fixed)
    if (m_p1->isFixed() && m_p2->isFixed()) return;


    if (m_p1->isRounded() && m_p2->isRounded()) {

	RoundedParticlePtr ball1Ptr = (RoundedParticlePtr) std::static_pointer_cast<RoundedParticle, Particle>(m_p1);
	RoundedParticlePtr ball2Ptr = (RoundedParticlePtr) std::static_pointer_cast<RoundedParticle, Particle>(m_p2);


	//Compute interpenetration distance
	float particleRoundedParticleDist = glm::distance(ball1Ptr->getPosition(),ball2Ptr->getPosition());
	float interpenetrationDist = ball1Ptr->getRadius()+ball2Ptr->getRadius()-particleRoundedParticleDist;

	//Compute particle-particle vector
	glm::vec3 k = glm::normalize(ball1Ptr->getPosition()-ball2Ptr->getPosition());

	//Project each particle along the particle-particle vector with half of the interpenetration distance
	//To be more precise, we ponderate the distance with the mass of the particle
	glm::vec3 prev_x1 = ball1Ptr->getPosition();
	glm::vec3 prev_x2 = ball2Ptr->getPosition();
	if(ball1Ptr->isFixed())
	    {
		ball2Ptr->setPosition(prev_x2 - interpenetrationDist*k);
	    }
	else if(ball2Ptr->isFixed())
	    {
		ball1Ptr->setPosition(prev_x1 + interpenetrationDist*k);
	    }
	else
	    {
		float c1 = ball1Ptr->getMass()/(ball1Ptr->getMass()+ball2Ptr->getMass());
		float c2 = ball2Ptr->getMass()/(ball1Ptr->getMass()+ball2Ptr->getMass());
		ball1Ptr->setPosition(prev_x1 + c2*interpenetrationDist*k);
		ball2Ptr->setPosition(prev_x2 - c1*interpenetrationDist*k);
	    }

	//Compute post-collision velocity
	glm::vec3 prev_v1 = ball1Ptr->getVelocity();
	glm::vec3 prev_v2 = ball2Ptr->getVelocity();
	float proj_v = (1.0f+m_restitution)*glm::dot(k, prev_v1-prev_v2)/(1.0 / ball1Ptr->getMass() + 1.0 / ball2Ptr->getMass());
	glm::vec3 new_v1 = prev_v1 - proj_v/ball1Ptr->getMass()*k;
	glm::vec3 new_v2 = prev_v2 + proj_v/ball2Ptr->getMass()*k;
	ball1Ptr->setVelocity(new_v1);
	ball2Ptr->setVelocity(new_v2);
    }
}


bool testParticleParticle(const ParticlePtr &p1, const ParticlePtr &p2)
{
    /* p1( c1, r1 ) and p2( c2, r2 ) particles
   * intersection if || c2 - c1 || <= r1 + r2.
   * We avoid as much as possible to compute of a square root, so we check if
   * dot( c2 - c1, c2 - c1 ) <= (r1 + r2)²
   */

    //Vector between sphere centers
    if(p1==p2) return false;

    if (p1->isRounded() && p2->isRounded()) {

	RoundedParticlePtr ball1Ptr = (RoundedParticlePtr) std::static_pointer_cast<RoundedParticle, Particle>(p1);
	RoundedParticlePtr ball2Ptr = (RoundedParticlePtr) std::static_pointer_cast<RoundedParticle, Particle>(p2);


	//Sum of sphere radii
	float r = ball1Ptr->getRadius() + ball2Ptr->getRadius();
	float c = glm::distance2(ball1Ptr->getPosition(), ball2Ptr->getPosition()) - r*r;
	return (c<0.0f) ? true : false;
    }

    return false;
}
