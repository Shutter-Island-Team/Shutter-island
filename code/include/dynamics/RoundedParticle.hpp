#ifndef ROUNDEDPARTICLE_HPP
#define ROUNDEDPARTICLE_HPP

#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include "Particle.hpp"


/**
 * @brief Represent a particle as a moving ball.
 *
 */
class RoundedParticle : public Particle
{
public:
    /**
     * @brief Instantiate a dynamic ball.
     *
     *
     * @param position The initial position.
     * @param velocity The initial speed.
     * @param mass The particle mass.
     * @param radius The particle radius.
     */
    RoundedParticle(
	     const glm::vec3& position,
	     const glm::vec3& velocity,
	     const float& mass,
	     const float& radius);
    ~RoundedParticle();

    /**
     * @brief Change the particle's radius
     *
     * @param the new radius
     */
    void setRadius(const float &radius);
    
    /**@brief Access to this particle's radius.
     *
     * Get the radius of this particle.
     * @return The particle's radius.
     */
    float getRadius() const;
    
private:
    
    /**@brief The particle's radius.
     *
     * The radius of this particle.
     */
    float m_radius;
};

typedef std::shared_ptr<RoundedParticle> RoundedParticlePtr;


#endif

