#ifndef DAMPING_FORCE_FIELD_HPP
#define DAMPING_FORCE_FIELD_HPP

#include <vector>
#include "ForceField.hpp"
#include "Particle.hpp"

/**@brief Implement a damping force field.
 *
 * This class implement a force field that models a damping, i.e.
 * a kinetic friction.
 */
class DampingForceField : public ForceField
{
    public:
        /**@brief Build a damping force field.
         *
         * Build a damping force applied to a set of particles. The force added
         * to a particle of velocity v is -damping * v.
         * @param particles Set of particles influenced by this damping force.
         * @param damping Damping coefficient. */
        DampingForceField(const std::vector<ParticlePtr> particles, const float damping);

        /**@brief Access to the particles influenced by this force field.
         *
         * Get the particles influenced by this damping force field.
         * @return The set of particles influenced by this.
         */
        const std::vector<ParticlePtr> getParticles();

        /**@brief Define the set of particles influenced by this force field.
         *
         * Define the set of particles that will be influenced by this damping
         * force force.
         * @param particles The new set of influenced particles.
         */
        void setParticles(const std::vector<ParticlePtr>& particles);

        /**@brief Access to the damping factor.
         *
         * Get the damping factor of this force field.
         * @return The damping factor of this.
         */
        const float& getDamping();
        /**@brief Set the damping factor of this force field.
         *
         * Define the damping factor of this damping force field.
         * @param damping The new damping factor.
         */
        void setDamping(const float& damping);

    private:
        void do_addForce();
        std::vector<ParticlePtr> m_particles;
        float m_damping;
};

typedef std::shared_ptr<DampingForceField> DampingForceFieldPtr;

#endif // SPRING_HPP

