#ifndef CONSTANT_FORCE_FIELD_HPP
#define CONSTANT_FORCE_FIELD_HPP

#include <vector>
#include "ForceField.hpp"
#include "Particle.hpp"

/**@brief Constant force field.
 *
 * Implementation of a force field that is constant, i.e. the same for
 * all managed particles.
 */
class ConstantForceField : public ForceField
{
    public:
        /**@brief Build a constant force field.
         *
         * Build a constant force field for a set of particles.
         * @param particles The set of particles influenced by this force field.
         * @param force The constant force applied to all particles.
         */
        ConstantForceField(const std::vector<ParticlePtr>& particles, const glm::vec3& force);

        /**@brief Access to the set of managed particles.
         *
         * Get the set of managed particles of this constant force field.
         * @return The managed force field.
         */
        const std::vector<ParticlePtr> getParticles();

        /**@brief Define a new set of particles managed by this constant force field.
         *
         * Set the particles influenced by this constant force field.
         * @param particles The new set of particles.
         */
        void setParticles(const std::vector<ParticlePtr>& particles);

        /**@brief Access to the force applied to all influenced particles.
         *
         * Get the constant force of this force field.
         * @return The force of this force field.
         */
        const glm::vec3& getForce();

        /**@brief Set the force applied to all influenced particles.
         *
         * Set the force applied to all particles influenced by this force field.
         * @param force The new force.
         */
        void setForce(const glm::vec3& force);

    private:
        void do_addForce();
        std::vector<ParticlePtr> m_particles;
        glm::vec3 m_force;
};

typedef std::shared_ptr<ConstantForceField> ConstantForceFieldPtr;

#endif // SPRING_HPP

