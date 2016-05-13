#ifndef SPRING_FORCEFIELD_RENDERABLE_HPP
#define SPRING_FORCEFIELD_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "SpringForceField.hpp"

/**@brief Render a spring force field.
 *
 * Render a spring force field on screen, or, equivalently, a spring between two
 * particles. If you have more than one spring to render, it could be more efficient
 * to use the class SpringListRenderable.
 */
class SpringForceFieldRenderable : public HierarchicalRenderable
{
public:
    ~SpringForceFieldRenderable();
    /**@brief Build a renderable to render a spring.
     *
     * Build a new renderable to render a spring between two particles.
     * @param program The shader program used to render the spring.
     * @param springForceField The force field to model the spring we want to render.
     */
    SpringForceFieldRenderable( ShaderProgramPtr program, SpringForceFieldPtr springForceField );

private:
    void do_draw();
    void do_animate( float time );

    SpringForceFieldPtr m_springForceField;

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
};

typedef std::shared_ptr<SpringForceFieldRenderable> SpringForceFieldRenderablePtr;

#endif // SPRING_RENDERABLE_HPP

