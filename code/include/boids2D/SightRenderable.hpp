#ifndef CUBE_RENDERABLE_HPP
#define CUBE_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "MovableBoid.hpp"
#include <vector>
#include <glm/glm.hpp>

class SightRenderable : public HierarchicalRenderable
{
    public:
        ~SightRenderable();
        SightRenderable();
        SightRenderable( ShaderProgramPtr program, MovableBoidPtr boid );

    private:
        void do_draw();
        void do_animate( float time );
        void do_keyPressedEvent( sf::Event& e);

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;

        MovableBoidPtr m_boid;
        bool m_display;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
};

typedef std::shared_ptr<SightRenderable> SightRenderablePtr;

#endif