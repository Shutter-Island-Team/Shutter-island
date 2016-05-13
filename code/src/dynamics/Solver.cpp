# include "../../include/dynamics/Solver.hpp"

void Solver::solve( const float& dt, std::vector<ParticlePtr>& particles )
{
  do_solve( dt, particles );
}
