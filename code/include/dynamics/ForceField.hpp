#ifndef FORCE_FIELD_HPP
#define FORCE_FIELD_HPP

#include <memory>

/**@brief Force field interface.
 *
 * Define an interface for a force field. A force field applies forces
 * to a set of handled particles. Those particles are stored in derived classes.
 */
class ForceField
{
public:
  ForceField();
  virtual ~ForceField();

  /**@brief Add a force to particles.
   *
   * Add a force to the particles influenced by this force field.
   */
  void addForce();
private:
  /**@brief Add force implementation.
   *
   * The actual implementation to add force to the particles.
   * This should be implemented in derived classes.
   */
  virtual void do_addForce() = 0;
};

typedef std::shared_ptr<ForceField> ForceFieldPtr;

#endif //FORCE_FIELD
