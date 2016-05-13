#ifndef SPHERE_RENDERABLE_HPP
#define SPHERE_RENDERABLE_HPP

#include "Renderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class SphereRenderable : public Renderable
{
    public:
        ~SphereRenderable();
        SphereRenderable( ShaderProgramPtr program );

    private:
        void do_draw();
        void do_animate( float time );

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
};

typedef std::shared_ptr<SphereRenderable> SphereRenderablePtr;

#endif
