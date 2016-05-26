#ifndef SOLVER_BOID_HPP
#define SOLVER_BOID_HPP

#include <vector>

#include "BoidsManager.hpp"


/**@brief Dynamic system solver interface for boids.
 *
 * Define an interface for dynamic system solver of boids.
 */
class SolverBoid
{
public:
  SolverBoid();

  /**@brief Solve the dynamic system of boids.
   *
   * Solve the dynamic system of boids for a specified time step.
   * @param dt 				The time step for the integration.
   * @param boidsManager 	The manager of boids.
   */
  void solve( const float& dt, BoidsManagerPtr boidsManager );
};

typedef std::shared_ptr<SolverBoid> SolverBoidPtr;

#endif