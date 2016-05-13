#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <memory>

/**@brief Interface of a collision between dynamic objects.
 *
 * This class store a collision event between two objects that
 * should be resolved to avoid object inter-penetration.
 */
class Collision
{
public:
    virtual ~Collision();
    /**@brief Build a collision event.
     *
     * Build a collision event.
     * @param restitution Restitution factor of this collision.
     */
    Collision(float restitution);

    /**@brief Solve this collision.
     *
     * Solve this collision to update the positions and velocities of objects
     * that collided.
     */
    void solveCollision();
private:
    /**@brief Implementation of the collision solving.
     *
     * Actual implementation of the collision solving, that should be done
     * in derived classes.
     */
    virtual void do_solveCollision() = 0;
protected:
    float m_restitution;
};

typedef std::shared_ptr<Collision> CollisionPtr;

#endif //COLLISION_HPP
