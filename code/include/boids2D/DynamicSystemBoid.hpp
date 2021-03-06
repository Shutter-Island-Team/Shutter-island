#ifndef DYNAMIC_SYSTEM_BOID_HPP
#define DYNAMIC_SYSTEM_BOID_HPP

#include "SolverBoid.hpp"
#include "BoidsManager.hpp"

/**
 * @class DynamicSystemBoid 
 * @brief Dynamic handler for the boidManager
 */
class DynamicSystemBoid
{
private:
   /**@brief The set a boid manager for this system.
    *
    * The boid manager managed by this dynamic system. Their positions
    * and velocities will be updated thanks to the solver, taking into
    * account the force field applied to them.
    */
    BoidsManagerPtr m_boidsManager;

    /**@brief The solver of the dynamic system.
     *
     * Solver of the dynamic system: update the boids positions and
     * velocities.
     */
    SolverBoidPtr m_solver;

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
    // std::vector<CollisionPtr> m_collisions;

public:
    DynamicSystemBoid();

    /**@brief Access to the solver used to resolve the dynamic system.
     *
     * Get the dynamic system solver used by this system.
     * @return The current dynamic system solver.
     */
    SolverBoidPtr getSolver();

    /**@brief Set a new dynamic system solver.
     *
     * Define a new solver to resolve the dynamic system at each simulation step.
     * @param solver The new solver to use.
     */
    void setSolver(SolverBoidPtr solver);

    /**@brief Set the boid manager of this system.
     *
     * Define a new set of boid manager for this dynamic system.
     * @param boidsManager The new set of boid manager of this system.
     */
    void setBoidsManager(BoidsManagerPtr boidsManager);

    /**@brief Compute a simulation step for this system.
     *
     * Compute a simulation step for this dynamic system, i.e. what happens
     * since the last simulation step and during the time integration interval.
     */
    void computeSimulationStep();

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
};

typedef std::shared_ptr<DynamicSystemBoid> DynamicSystemBoidPtr;

#endif