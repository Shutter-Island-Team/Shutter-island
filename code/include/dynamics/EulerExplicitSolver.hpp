#ifndef EULER_EXPLICIT_SOLVER_HPP
#define EULER_EXPLICIT_SOLVER_HPP

#include "Solver.hpp"

/**@brief Explicit Euler solver.
 *
 * Explicit Euler dynamic system solver.
 */
class EulerExplicitSolver : public Solver
{
public:
    EulerExplicitSolver();
    ~EulerExplicitSolver();
private:
    void do_solve(const float& dt, std::vector<ParticlePtr>& particles);
};

typedef std::shared_ptr<EulerExplicitSolver> EulerExplicitSolverPtr;

#endif //EULER_EXPLICIT_SOLVER_HPP
