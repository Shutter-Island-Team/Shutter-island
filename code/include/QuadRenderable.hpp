#ifndef QUAD_RENDERABLE_HPP
#define QUAD_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include "Plane.hpp"

#include <vector>
#include <glm/glm.hpp>

/**@brief Render a quad on the screen.
 *
 * This class is used to render a quad on the screen. */
class QuadRenderable : public HierarchicalRenderable
{
public:
    ~QuadRenderable();
    /**@brief Build a quad passing by four vertices.
     *
     * Build a quad passing by four vertices. The quad is rendering
     * thanks to two triangles: (p1, p2, p3) and (p1, p3, p4).
     * @param program Shader program used to render this quad.
     * @param p1 First quad vertex.
     * @param p2 Second quad vertex.
     * @param p3 Third quad vertex.
     * @param p4 Fourth quad vertex.
     * @param color Color of the quad. */
    QuadRenderable(ShaderProgramPtr program,
                    const glm::vec3& p1,
                    const glm::vec3& p2,
                    const glm::vec3& p3,
                    const glm::vec3& p4,
                    const glm::vec4& color = glm::vec4{1.0, 0, 0, 1.0}
                    );

private:
    void do_draw();
    void do_animate( float time );

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
};

typedef std::shared_ptr<QuadRenderable> PlaneRenderablePtr;

#endif //PLANE_RENDERABLE_HPP
