#ifndef MULTI_TEXTURED_CUBE__RENDERABLE_HPP
#define MULTI_TEXTURED_CUBE__RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

class MultiTexturedCubeRenderable : public HierarchicalRenderable
{
public :
    ~MultiTexturedCubeRenderable();
    MultiTexturedCubeRenderable(ShaderProgramPtr shaderProgram, const std::string &filename1, const std::string& filename2);

    void setMaterial(const MaterialPtr& material);

private:
    void do_draw();
    void do_animate( float time );

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;
    std::vector< glm::vec2> m_texCoords1, m_texCoords2;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
    unsigned int m_tBuffer1, m_tBuffer2;
    unsigned int m_texId1, m_texId2;
    float m_blendingCoefficient;

    MaterialPtr m_material;
};

typedef std::shared_ptr<MultiTexturedCubeRenderable> MultiTexturedCubeRenderablePtr;

#endif
