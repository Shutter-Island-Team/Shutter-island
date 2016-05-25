#ifndef SOLVER_BOID_HPP
#define SOLVER_BOID_HPP

#include <vector>

#include "Boid.hpp"
#include "../../include/boids2D/MovableBoid.hpp"


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
   * @param dt The time step for the integration.
   * @param boids The collection of boids.
   */
  void solve( const float& dt, std::vector<MovableBoidPtr>& boids );
};

typedef std::shared_ptr<SolverBoid> SolverBoidPtr;

#endif