#ifndef DYNAMIC_SYSTEM_BOID_RENDERABLE_HPP
#define DYNAMIC_SYSTEM_BOID_RENDERABLE_HPP

#include "../../include/HierarchicalRenderable.hpp"
#include "DynamicSystemBoid.hpp"


/**@brief A little hack to incorporate the dynamic system.
 *
 * This class is mostly a hack we use to deal with the dynamic system components
 * inside the framework. We could (should) do that differently, but this hack was
 * considered to be simpler for the students to understand, since it reuse the
 * concept of renderable. Moreover, since it is a hierarchical renderable, it is
 * possible to define a dynamic system in a local frame and then use hierarchical
 * geometric transformation to replace it correctly in the scene.
 */
class DynamicSystemBoidRenderable : public HierarchicalRenderable
{
public:
    /**@brief Build a dynamic system renderable.
     *
     * Build a dynamic system renderable from a dynamic system. The dynamic system
     * will them be updated by this renderable. It allows us also to interact with
     * the dynamic system components thanks to user input events.
     * @param system The dynamic system to manage by this renderable.
     */
    DynamicSystemBoidRenderable(DynamicSystemBoidPtr system);

    /**@brief Change the managed dynamic system.
     *
     * Change the dynamic system that will be managed by this renderable.
     * @param system The new dynamic system managed by this.
     */
    void setDynamicSystem(const DynamicSystemBoidPtr &system);

private:
    void do_draw();
    /**@brief Update the dynamic system.
     *
     * This function will update the managed dynamic system, i.e. compute the
     * new positions and velocities of the particles. This update will be
     * actually done only if the last time the system was updated was more
     * than m_sytem->m_dt ago. This allows us to update the dynamic system
     * at the specified time interval.
     */
    void do_animate( float time );

    /**@brief Dynamic system managed by this renderable.
     *
     * The dynamic system that is managed by this renderable, to compute
     * the simulation steps.
     */
    DynamicSystemBoidPtr m_system;

    /**@brief Last time the dynamic system was updated.
     *
     * Store the last time the dynamic system was updated. This is used to
     * keep updating the dynamic system at the specified time interval.
     */
    float m_lastUpdateTime;
};

typedef std::shared_ptr<DynamicSystemBoidRenderable> DynamicSystemBoidRenderablePtr;

#endif