#ifndef SOLVER_BOID_HPP
#define SOLVER_BOID_HPP

#include <vector>

#include "Boid.hpp"

/**@brief Dynamic system solver interface for boids.
 *
 * Define an interface for dynamic system solver of boids.
 */
class SolverBoid
{
public:
  SolverBoid();

  /**@brief Solve the dynamic system of particles.
   *
   * Solve the dynamic system of particles for a specified time step.
   * @param dt The time step for the integration.
   * @param particles The collection of particles.
   */
  void solve( const float& dt, std::vector<BoidPtr>& boids );
};

typedef std::shared_ptr<SolverBoid> SolverBoidPtr;

#endif