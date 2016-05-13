#ifndef STICKPARTICLE_HPP
#define STICKPARTICLE_HPP

#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include "../Viewer.hpp"
#include "Particle.hpp"


/**
 * @brief Represent a particle as a stick (oriented particle)
 *
 */

class StickParticle : public Particle
{
public:
    /**
     * @brief Instantiate a dynamic stick
     *
     *
     * @param position The initial position.
     * @param velocity The initial speed.
     * @param mass The particle mass.
     * @param p1 The first particle defining the orientation
     * @param p2 The second particle defining the orientation
     */
    StickParticle(
		  const glm::vec3& position,
		  const glm::vec3& velocity,
		  const float& mass,
		  const ParticlePtr &p1,
		  const ParticlePtr &p2);
    ~StickParticle();
    
    /**
     * @brief Access to this particle's orientation
     *
     * @return The particle's orientation
     */
    glm::vec3 getOrientation() const;

    /**
     * @brief Update of the position and the velocity of the hierarchy
     */
    void updateHierarchy();


    /**
     * @brief Set if the particle follows its children or the opposite
     */
    void setControlChild1(bool control);
    void setControlChild2(bool control);
    void setControlChildren(bool control);


    /**
     * @brief Handle recursively the collision
     * Could be used but a lot of "Hierarchical" Particles
     */
    void setCollision(float penetration, glm::vec3 collisionDirection);
    void handleCollision(float penetration, glm::vec3 collisionDirection);

private:
    
    /**
     *@brief The particle's orientation
     */
    glm::vec3 m_orientation;
    ParticlePtr m_p1;
    ParticlePtr m_p2;

    /**
     * @brief Variables to control if the stick controls its children
     * or the opposite
     * Default : both of the children describe the father
     */
    bool m_controlChild1 = false;
    bool m_controlChild2 = false;
    /**
     * @brief Relative positions of the children
     */
    glm::vec3 m_p1_pos;
    glm::vec3 m_p2_pos;

    /**
     * @brief Variable to handle the collision
     * TODO : Move this structure somewhere else 
     * (DynamicHierarchicalRenderable that stores in
     * the particle a ref to the Renderable ?)
     */
    bool m_collisionToHandle = false;
    float m_penetration;
    glm::vec3 m_collisionDirection;

protected:
    int m_collision = 0;

};

typedef std::shared_ptr<StickParticle> StickParticlePtr;


#endif
