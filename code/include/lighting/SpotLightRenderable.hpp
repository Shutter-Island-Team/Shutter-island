#ifndef SPOT_LIGHT_RENDERABLE_HPP
#define SPOT_LIGHT_RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "Light.hpp"

#include <vector>
#include <glm/glm.hpp>

class SpotLightRenderable : public HierarchicalRenderable
{
public:
    ~SpotLightRenderable();
    SpotLightRenderable( ShaderProgramPtr program, SpotLightPtr light);

private:
    void do_draw();
    void do_animate( float time );

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;

    SpotLightPtr m_light;
};

typedef std::shared_ptr<SpotLightRenderable> SpotLightRenderablePtr;

#endif
