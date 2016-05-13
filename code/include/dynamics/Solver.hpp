#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <memory>
#include <vector>
#include "Particle.hpp"

/**@brief Dynamic system solver interface.
 *
 * Define an interface for dynamic system solver.
 */
class Solver
{
public:
  Solver(){}
  virtual  ~Solver(){}
  /**@brief Solve the dynamic system of particles.
   *
   * Solve the dynamic system of particles for a specified time step.
   * @param dt The time step for the integration.
   * @param particles The collection of particles.
   */
  void solve( const float& dt, std::vector<ParticlePtr>& particles );
private:
  /**@brief Solve implementation.
   *
   * The actual implementation to solve the dynamic system. This should
   * be implemented in derived classes.
   * @param dt The time step for the integration.
   * @param particles The collection of particles.
   */
  virtual void do_solve(const float& dt, std::vector<ParticlePtr>& particles) = 0;
};

typedef std::shared_ptr<Solver> SolverPtr;

#endif //SOLVER_HPP
