#ifndef SPRING_FORCE_FIELD_HPP
#define SPRING_FORCE_FIELD_HPP

#include "ForceField.hpp"
#include "Particle.hpp"

/**@brief Implement a spring force field.
 *
 * This class implements a force field to model a spring force
 * between two particles.
 */
class SpringForceField : public ForceField
{
    public:
        /**@brief Build a new spring force field.
         *
         *  Build a new spring force field between two particles.
         *  @param p1 the first particle of this spring
         *  @param p2 the second particle of this spring
         *  @param stiffness spring stiffness
         *  @param equilibriumLength equilibrium length
         *  @param damping damping factor
         */
        SpringForceField(const ParticlePtr p1, const ParticlePtr p2,
                float stiffness, float equilibriumLength, float damping);


        /**@brief Access to the first particle connected to this spring.
         *
         * Get the first particle connected to this spring.
         * @return The first particle connected to this spring.
         */
        ParticlePtr getParticle1() const;
        /**@brief Access to the second particle connected to this spring.
         *
         * Get the second particle connected to this spring.
         * @return The second particle connected to this spring.
         */
        ParticlePtr getParticle2() const;

    private:
        /**@brief Add the force of this spring to the two particles.
         *
         * Compute the forces applied by this spring to each particles
         * and add them to the particles.
         */
        void do_addForce();


        const ParticlePtr m_p1, m_p2;
        float m_stiffness;
        float m_equilibriumLength;
        float m_damping;
};

typedef std::shared_ptr<SpringForceField> SpringForceFieldPtr;

#endif // SPRING_HPP

