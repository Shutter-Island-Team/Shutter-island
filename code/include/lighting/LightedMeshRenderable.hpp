#ifndef LIGHTED_MESH_RENDERABLE_HPP
#define LIGHTED_MESH_RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "./../lighting/Light.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

class LightedMeshRenderable : public HierarchicalRenderable
{
    public:
        ~LightedMeshRenderable();
        LightedMeshRenderable(ShaderProgramPtr program, const std::string& filename);
        void setMaterial(const MaterialPtr& material);

    private:
        void do_draw();
        void do_animate( float time );

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec3 > m_normals;
        std::vector< glm::vec4 > m_colors;
        std::vector< unsigned int > m_indices;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
        unsigned int m_iBuffer;

        MaterialPtr m_material;
};

typedef std::shared_ptr<LightedMeshRenderable> LightedMeshRenderablePtr;

#endif
