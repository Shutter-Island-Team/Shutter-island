#ifndef SPRING_LIST_RENDERABLE_HPP
#define SPRING_LIST_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "SpringForceField.hpp"
#include <list>
#include <vector>


/**@brief Render a collection of springs.
 *
 * Render a set of springs on screen. We could do much more here than just
 * rendering a line between the centers of the two spring's particles.
 * However, it is up to you to come with better idea :-).
 */
class SpringListRenderable : public HierarchicalRenderable
{
public:
    ~SpringListRenderable();
    /**@brief Build a renderable to render a list of springs.
     *
     * Build a new renderable to render a list of springs.
     * @param program The shader program used to render the springs.
     * @param springForceFields The set of springs force fields that model
     * the springs we want to render.
     */
    SpringListRenderable( ShaderProgramPtr program, std::list<SpringForceFieldPtr>& springForceFields );

private:
    void do_draw();
    void do_animate( float time );

    std::list<SpringForceFieldPtr> m_springForceFields;

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
};

typedef std::shared_ptr<SpringListRenderable> SpringListRenderablePtr;

#endif // SPRING_LIST_RENDERABLE_HPP

