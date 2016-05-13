#ifndef DIRECTIONAL_LIGHT_RENDERABLE_HPP
#define DIRECTIONAL_LIGHT_RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "Light.hpp"

#include <vector>
#include <glm/glm.hpp>

class DirectionalLightRenderable : public HierarchicalRenderable
{
public:
    ~DirectionalLightRenderable();
    DirectionalLightRenderable( ShaderProgramPtr program, DirectionalLightPtr light, const glm::vec3& position);
    glm::vec3 position() const;
    void setPosition(const glm::vec3 &position);

private:
    void do_draw();
    void do_animate( float time );

    glm::vec3 m_position;

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;

    DirectionalLightPtr m_light;
};

typedef std::shared_ptr<DirectionalLightRenderable> DirectionalLightRenderablePtr;

#endif
