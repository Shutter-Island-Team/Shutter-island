#ifndef TEXTURED_PLANE_RENDERABLE_HPP
#define TEXTURED_PLANE_RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

class TexturedPlaneRenderable : public HierarchicalRenderable
{
public :
    ~TexturedPlaneRenderable();
    TexturedPlaneRenderable(ShaderProgramPtr shaderProgram, const std::string& filename);
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
    std::vector< glm::vec2 > m_origTexCoords;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
    unsigned int m_tBuffer;
    unsigned int m_texId;

    unsigned int m_wrapOption;
    unsigned int m_filterOption;

    MaterialPtr m_material;
};

typedef std::shared_ptr<TexturedPlaneRenderable> TexturedPlaneRenderablePtr;

#endif
