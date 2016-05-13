#ifndef CYLINDER_RENDERABLE_HPP
#define CYLINDER_RENDERABLE_HPP

#include "Renderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class CylinderRenderable : public Renderable
{
public:
    ~CylinderRenderable();
    CylinderRenderable( ShaderProgramPtr program );

    void switchNormals();

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

typedef std::shared_ptr<CylinderRenderable> CylinderRenderablePtr;

#endif
