#ifndef BOXPARTICLE_HPP
#define BOXPARTICLE_HPP

#include "StickParticle.hpp"



/**
 * @brief Represent a particle as a box (oriented particle + dimensions)
 */

class BoxParticle : public StickParticle
{
public:
    /**
     * @brief Instantiate a dynamic box
     *
     *
     * @param position The initial position.
     * @param velocity The initial speed.
     * @param mass The particle mass.
     * @param length The particle length
     * @param width The particle width
     * @param height The particle height
     * @param p1 The first particle defining the orientation
     * @param p2 The second particle defining the orientation
     */
    BoxParticle(const glm::vec3& position,
		const glm::vec3& velocity,
		const float& mass,
		const float &length,
		const float &width,
		const float &height,
		const ParticlePtr &p1,
		const ParticlePtr &p2);
    ~BoxParticle();

    /**
     * @brief Get the dimensions
     */
    float getLength();
    float getWidth();
    float getHeight();

private:
    /**
     * @brief The particle dimensions
     */
    float m_length;
    float m_width;
    float m_height;
};

typedef std::shared_ptr<BoxParticle> BoxParticlePtr;

#endif
