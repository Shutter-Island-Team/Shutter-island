#ifndef PARTICLE_PARTICLE_COLLISION_HPP
#define PARTICLE_PARTICLE_COLLISION_HPP

#include "Collision.hpp"
#include "Particle.hpp"
#include "RoundedParticle.hpp"

/**@brief Implement the resolution of a collision event between two particles.
 *
 * Implementation of the resolution of a collision between two particles.
 */
class ParticleParticleCollision : public Collision
{
public:
    ~ParticleParticleCollision();
    /**@brief Build a new collision event between two particles.
     *
     * Build a collision event between two particles.
     * @param particle1 The first colliding particle.
     * @param particle2 The second colliding particle.
     * @param restitution The restitution factor of the collision.
     */
    ParticleParticleCollision(ParticlePtr particle1, ParticlePtr particle2, float restitution);
private:
    void do_solveCollision();
    ParticlePtr m_p1;
    ParticlePtr m_p2;
};

typedef std::shared_ptr<ParticleParticleCollision> ParticleParticleCollisionPtr;

bool testParticleParticle(const ParticlePtr& p1, const ParticlePtr& p2);

#endif //PARTICLE_PARTICLE_COLLISION_HPP
