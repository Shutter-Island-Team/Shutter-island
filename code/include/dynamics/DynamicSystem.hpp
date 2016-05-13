#ifndef DYNAMICSYSTEM_HPP
#define DYNAMICSYSTEM_HPP

#include <vector>

#include "Collision.hpp"
#include "ForceField.hpp"
#include "Particle.hpp"
#include "Solver.hpp"
#include "../Plane.hpp"

/**@brief A dynamic system.
 *
 * This class represents a dynamic system made of particles, force fields and
 * fixed planes obstacles and that handle collisions. If you want to, you can
 * replace fixed planes obstacles by triangle obstacles: you will be able to
 * model more kind of obstacles. However, this would require a spatial optimization
 * that is out of the scope of these practical lessons.
 */
class DynamicSystem
{
private:
  /**@brief The set of particles managed by this system.
   *
   * The particles managed by this dynamic system. Their positions
   * and velocities will be updated thanks to the solver, taking into
   * account the force field applied to them.
   */
    std::vector<ParticlePtr> m_particles;

    /**@brief The set of force fields influencing particles of this system.
     *
     * The force fields that influence the particles of this system.
     */
    std::vector<ForceFieldPtr> m_forceFields;

    /**@brief The set of fixed plane obstacles.
     *
     * The set of obstacles that would repel the particles after collisions.
     */
    std::vector<PlanePtr> m_planeObstacles;

    /**@brief The solver of the dynamic system.
     *
     * Solver of the dynamic system: update the particles positions and
     * velocities according to the force fields applied.
     */
    SolverPtr m_solver;

    /**@brief Time interval used for integration.
     *
     * This represent the time step at which the dynamic system will be
     * updated. A large time step would leads to less robust behaviors,
     * while smaller time steps require more computation power for the
     * same length of simulation.
     */
    float m_dt;

    /**@brief The set of collision events detected during a simulation step.
     *
     * Set of collision events between dynamic components during a simulation
     * step. Those events would be resolved by updating velocities and positions
     * of dynamic objects to avoid inter-penetration.
     */
    std::vector<CollisionPtr> m_collisions;

    /**@brief A flag to activate/desactivate collision detection.
     *
     * If set to false, collisions are ignored, leading to a faster simulation
     * but less realistic/interesting. When set to true, collisions are detected
     * and resolved.
     */
    bool m_handleCollisions;
    /**@brief Restitution factor of collisions.
     *
     * The factor of restitution after a collision between objects.
     */
    float m_restitution;

public:
    ~DynamicSystem();
    DynamicSystem();

    /**@brief Add a particle to the system.
     *
     * Add a particle to this dynamic system.
     * @param p The particle to add to this system.
     */
    void addParticle(ParticlePtr p);
    /**@brief Add a force field to the system.
     *
     * Add a force field to this dynamic system to influence particles.
     * @param forceField The force field to add to this system.
     */
    void addForceField(ForceFieldPtr forceField);
    /**@brief Add a plane obstacle to the system.
     *
     * Add an infinite plane obstacle to the dynamic system. If collisions
     * are activated, this plane will repel particles.
     * @param planeObstacle The plane to add to this system.
     */
    void addPlaneObstacle(PlanePtr planeObstacle);

    /**@brief Access to the solver used to resolve the dynamic system.
     *
     * Get the dynamic system solver used by this system.
     * @return The current dynamic system solver.
     */
    SolverPtr getSolver();
    /**@brief Set a new dynamic system solver.
     *
     * Define a new solver to resolve the dynamic system at each simulation step.
     * @param solver The new solver to use.
     */
    void setSolver(SolverPtr solver);

    /**@brief Check if the collision detection is activated.
     *
     * Check if the collision are currently handled by this dynamic system.
     * @return True if the collisions are handled.
     */
    bool getCollisionDetection();
    /**@brief Set the collision detection mode.
     *
     * Define if the collisions are detected/handled by this dynamic system.
     * @param onOff True if the collision should be detected/handled.
     */
    void setCollisionsDetection(bool onOff);

    /**@brief Access to the set of particles of this system.
     *
     * Get the set of particles of this dynamic system.
     * @return The set of particles of this system.
     */
    const std::vector<ParticlePtr>& getParticles() const;
    /**@brief Set the particles of this system.
     *
     * Define a new set of particles for this dynamic system.
     * @param particles The new set of particles of this system.
     */
    void setParticles(const std::vector<ParticlePtr> & particles);

    /**@brief Access to the force fields of this system.
     *
     * Get the set of force fields of this system.
     * @return The set of force fields of this system.
     */
    const std::vector<ForceFieldPtr>& getForceFields() const;
    /**@brief Set the force fields of this system.
     *
     * Define a new set of force fields for this dynamic system.
     * @param forceFields The new set of force fields of this dynamic system.
     */
    void setForceFields(const std::vector<ForceFieldPtr> &forceFields);


    /**@brief Compute a simulation step for this system.
     *
     * Compute a simulation step for this dynamic system, i.e. what happens
     * since the last simulation step and during the time integration interval.
     */
    void computeSimulationStep();

    /**@brief Access to the collision restitution factor.
     *
     * Get the current collision restitution factor of this system.
     * @return The current collision restitution factor.
     */
    const float getRestitution();
    /**@brief Set the collision restitution factor.
     *
     * Define the new collision restitution factor for this dynamic system.
     * @param restitution The new collision restitution factor.
     */
    void setRestitution(const float &restitution);

    /**@brief Access the time integration interval.
     *
     * Get the time integration interval used by this dynamic system.
     * @return The time integration interval.
     */
    float getDt() const;

    /**@brief Set the time integration interval.
     *
     * Define the new time integration interval used by this dynamic system.
     * @param dt The new time integration interval.
     */
    void setDt(float dt);

    /**@brief Clear the dynamic system.
     *
     * Clear the system, i.e. empty the particles, force fields and plane obstacles.
     */
    void clear();

private:
    void detectCollisions();
    void solveCollisions();
};

typedef std::shared_ptr<DynamicSystem> DynamicSystemPtr;

/**
 * \brief output stream operator, as non-member
 */
std::ostream& operator<<(std::ostream& os, const DynamicSystemPtr& system);


#endif

