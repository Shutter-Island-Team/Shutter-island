#ifndef LIGHTED_CYLINDER_RENDERABLE_HPP
#define LIGHTED_CYLINDER_RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

class LightedCylinderRenderable : public HierarchicalRenderable
{
public:
    ~LightedCylinderRenderable();
    LightedCylinderRenderable( ShaderProgramPtr program );

    void setMaterial(const MaterialPtr& material);

private:
    void do_draw();
    void do_animate( float time );

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;

    MaterialPtr m_material;
};

typedef std::shared_ptr<LightedCylinderRenderable> LightedCylinderRenderablePtr;

#endif
