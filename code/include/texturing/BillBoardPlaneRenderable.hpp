#ifndef BILLBOARD_PLANE_RENDERABLE_HPP
#define BILLBOARD_PLANE_RENDERABLE_HPP

#include "./../Renderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

class BillBoardPlaneRenderable : public Renderable
{
public :
    ~BillBoardPlaneRenderable();
    BillBoardPlaneRenderable(
        ShaderProgramPtr shaderProgram,
        const std::string& texture_filename,
        const glm::vec3& billboardWorldPosition, const glm::vec2& billboardWorldDimension);
    void setMaterial(const MaterialPtr& material);


private:
    void do_draw();
    void do_animate( float time );
    void do_keyPressedEvent( sf::Event& e );
    void updateTextureOption();

    glm::vec2 m_billboardWorldDimension;
    glm::vec3 m_billboardWorldPosition;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec2 > m_shift;

    unsigned int m_cBuffer;
    unsigned int m_tBuffer;
    unsigned int m_texId;

    MaterialPtr m_material;
};

typedef std::shared_ptr<BillBoardPlaneRenderable> BillBoardPlaneRenderablePtr;

#endif
