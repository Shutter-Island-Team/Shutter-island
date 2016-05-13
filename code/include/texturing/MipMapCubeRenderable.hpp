#ifndef MIPMAP_CUBE__RENDERABLE_HPP
#define MIPMAP_CUBE__RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

class MipMapCubeRenderable : public HierarchicalRenderable
{
public :
    ~MipMapCubeRenderable();
    MipMapCubeRenderable(ShaderProgramPtr shaderProgram, std::vector<std::string>& filenames);

    void setMaterial(const MaterialPtr& material);

private:
    void do_draw();
    void do_animate( float time );
    void do_keyPressedEvent( sf::Event& e );
    void updateTextureOption();

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;
    std::vector< glm::vec2 > m_texCoords;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
    unsigned int m_tBuffer;
    unsigned int m_texId;

    unsigned int m_mipmapOption;

    MaterialPtr m_material;
};

typedef std::shared_ptr<MipMapCubeRenderable> MipMapCubeRenderablePtr;

#endif
