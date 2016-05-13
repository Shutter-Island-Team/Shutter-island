#ifndef CONSTANT_RENDERABLE_HPP
#define CONSTANT_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "ConstantForceField.hpp"

/**@brief Render a constant force field.
 *
 * This class is used to render a constant force field.
 */
class ConstantForceFieldRenderable : public HierarchicalRenderable
{
public:
    ~ConstantForceFieldRenderable();
    /**@brief Build a renderable for a constant force field.
     *
     * Create a renderable for a constant force field.
     * @param program Shader program to use to render the constant force field.
     * @param forceField The constant force field to render.
     */
    ConstantForceFieldRenderable( ShaderProgramPtr program, ConstantForceFieldPtr forceField);

private:
    void do_draw();
    void do_animate( float time );

    ConstantForceFieldPtr m_forceField;

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
};

typedef std::shared_ptr<ConstantForceFieldRenderable> ConstantForceFieldRenderablePtr;

#endif // CONSTANT_RENDERABLE_HPP

