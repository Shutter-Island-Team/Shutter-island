#ifndef MESH_RENDERABLE_HPP
#define MESH_RENDERABLE_HPP

#include "Renderable.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

class MeshRenderable : public Renderable
{
    public:
        ~MeshRenderable();
        MeshRenderable(ShaderProgramPtr program, const std::string& filename);

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
};

typedef std::shared_ptr<MeshRenderable> MeshRenderablePtr;

#endif
