#ifndef PARTICLE_PLANE_COLLISION_HPP
#define PARTICLE_PLANE_COLLISION_HPP

#include "Collision.hpp"
#include "Particle.hpp"
#include "RoundedParticle.hpp"
#include "BoxParticle.hpp"
#include "./../../include/Plane.hpp"

/**@brief Implement the resolution of a collision event between a plane and a particle.
 *
 * Implementation of the resolution of a collision event between a plane and a particle.
 */
class ParticlePlaneCollision : public Collision
{
public:
    ~ParticlePlaneCollision();
    /**@brief Build a new collision event between a plane and a particle.
     *
     * Build a collision event between a plane and a particle. This collision will
     * be resolved assuming the plane is fixed.
     * @param particle The particle colliding a plane.
     * @param plane The plane colliding a particle.
     * @param restitution Restitution factor of this collision.
     */
    ParticlePlaneCollision(ParticlePtr particle, PlanePtr plane, float restitution);
private:
    /**@brief Solve the collision between the plane and the particle.
     *
     * Update the particle position and velocity after its collision with
     * the fixed plane.
     */
    void do_solveCollision();
    ParticlePtr m_particle;
    PlanePtr m_plane;
};

typedef std::shared_ptr<ParticlePlaneCollision> ParticlePlaneCollisionPtr;

bool testParticlePlane(const ParticlePtr& particle, const PlanePtr& plane);

#endif //PARTICLE_PLANE_COLLISION_HPP
