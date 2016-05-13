#ifndef INDEXED_CUBE_RENDERABLE_HPP
#define INDEXED_CUBE_RENDERABLE_HPP

#include "Renderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class IndexedCubeRenderable : public Renderable
{
    public:
        ~IndexedCubeRenderable();
        IndexedCubeRenderable();
        IndexedCubeRenderable( ShaderProgramPtr program );

    private:
        void do_draw();
        void do_animate( float time );

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec3 > m_normals;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::ivec3 > m_indices;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
        unsigned int m_iBuffer;
};

typedef std::shared_ptr<IndexedCubeRenderable> IndexedCubeRenderablePtr;

#endif
