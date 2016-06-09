#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <iostream>
#include <memory>
#include <glm/glm.hpp>
/**
 * @brief Represent a particle affected by physics
 * 
 * This class is used to model particles in a dynamic system.
 * A particle has  mass, a velocity and a position. 
 * This particle is affected by forces that will change
 * both its position and its velocity. This particle can be fixed,
 * making its position constant and its velocity null, or be
 * a 'phantom' particle
 */

class Particle;
typedef std::shared_ptr<Particle> ParticlePtr;

class Particle 
{
public:
    /** 
     * @brief Instantiate a dynamic particle
     *
     * Initial positions and velocity are kept to reset the animation
     *
     * @param position The initial position 
     * @param velocity The initial speed
     * @param mass The particle mass
     */
    Particle(
	     const glm::vec3& position,
	     const glm::vec3& velocity,
	     const float& mass);
    ~Particle();

    /**@brief Access to this particle's position.
     *
     * Get the position of this particle.
     * @return The particle's position.
     */
    const glm::vec3& getPosition() const;

    glm::vec3 & getPosition();

    /**@brief Access to this particle's velocity.
     *
     * Get the velocity of this particle.
     * @return The particle's velocity.
     */
    const glm::vec3& getVelocity() const;
    /**@brief Access to this particle's applied force.
     *
     * Get the force applied to this particle.
     * @return The particle's applied force.
     */
    const glm::vec3& getForce() const;
    /**@brief Access to this particle's mass.
     *
     * Get the mass of this particle.
     * @return The particle's mass.
     */
    float getMass() const;

    /**@brief Check if this particle is fixed.
     *
     * Check if this particle is fixed.
     * @return True if the particle is fixed.
     */
    bool isFixed() const;

    /**@brief Set the particle's position.
     *
     * Set the position of this particle.
     * @param pos The new position of this particle.
     */
    void setPosition(const glm::vec3 &pos);
    /**@brief Set the particle's velocity.
     *
     * Set the velocity of this particle.
     * @param vel The new velocity of this particle.
     */
    void setVelocity(const glm::vec3 &vel);
    /**@brief Set the particle's applied force.
     *
     * Set the force applied to this particle.
     * @param force The new force applied to this particle.
     */
    void setForce(const glm::vec3 &force);
    /**@brief Set the particle's fixed flag.
     *
     * Set the fixed flag of this particle.
     * @param isFixed The new value of the fixed flag.
     */
    void setFixed(bool isFixed);

    /**@brief Increment the particle's position.
     *
     * Increment the position of this particle.
     * @param pos The position to add to this particle's position,
     * i.e. m_position += pos.
     */
    void incrPosition(const glm::vec3 &pos);
    /**@brief Increment the particle's velocity.
     *
     * Increment the velocity of this particle.
     * @param vel The velocity to add to this particle's velocity,
     * i.e. m_velocity += vel.
     */
    void incrVelocity(const glm::vec3 &vel);
    /**@brief Increment the particle's applied force.
     *
     * Increment the force applied to this particle.
     * @param force The force to add to this particle's applied force,
     * i.e. m_force += force.
     */
    void incrForce(const glm::vec3 &force);

    /**@brief Restart the particle.
     *
     * Set the particle's position and velocity to their initial values.
     */
    void restart();
    
    /**
     *@brief Check if this particle is dragged.
     *
     * @return True if the particle is dragged.
     */
    bool isDragged() const;
    /**
     * @brief Set the particle's dragged flag.
     */
    void setDragged(bool isDragged);
    
    
    /**
     * @brief Function called by the viewer to tell the particle
     * it's been followed
     */
    void setIsFollowedCamera(bool val);
    /**
     * @brief Functions to call to recompute the position of the camera
     * For now, the parameters to compute those parameters are constants
     * in the code, but they later should be passed as arguments
     */
    void recomputeCamera();
    /**
     * @brief Functions to get the camera parameters
     */
    glm::vec3 getCameraPosition();
    glm::vec3 getCameraTarget();

    /** 
     * @brief Functions to know how to downcast
     */
    bool isRounded();
    bool isAStick();
    bool isABox();
    
protected:
    /**@brief The initial particle's position.
     *
     * The initial position of this particle, stored to be set at restart.
     */
    const glm::vec3 m_initialPosition;
   
    /**@brief The particle's position.
     *
     * The position of this particle.
     */
    glm::vec3 m_position;

    /**@brief The initial particle's velocity.
     *
     * The initial velocity of this particle, stored to be set at restart.
     */
    const glm::vec3 m_initialVelocity;

    /**@brief The particle's velocity.
     *
     * The position of this particle.
     */
    glm::vec3 m_velocity;

    /**@brief The particle's applied force.
     *
     * The force applied to this particle.
     */
    glm::vec3 m_force;

    /**@brief The particle's mass.
     *
     * The mass of this particle.
     */
    float m_mass;

    /**@brief The particle's fixed flag.
     *
     * A flag that tells if this particle is fixed. If set to true,
     * the position should be constant and the velocity null in simulation steps.
     */
    bool m_isFixed = false;

    /**
     * @brief Fields to know how to downcast 
     */
    bool m_isRounded = false;
    bool m_isAStick = false;
    bool m_isABox = false;

    /**
     * @brief Fields to set the parameters for the camera to follow the particle
     */ 
    bool m_isFollowed = false;
    glm::vec3 m_cameraPosition = glm::vec3(0.0, 0.0, 10.0);
    glm::vec3 m_cameraDirection = glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 m_cameraTarget = glm::vec3(0.0, 0.0, 0.0);
    // Boolean used to have a smooth camera
    bool m_initCamera = false;
    glm::vec3 m_oldPosition = glm::vec3(0.0, 0.0, 0.0);
};



/**
 * \brief output stream operator, as non-member
 */
std::ostream& operator<<(std::ostream& os, const ParticlePtr& p);

#endif
