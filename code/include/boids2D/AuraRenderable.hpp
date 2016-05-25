#ifndef AURA_RENDERABLE_HPP
#define AURA_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "Boid.hpp"
#include <vector>
#include <glm/glm.hpp>

class AuraRenderable : public HierarchicalRenderable
{
    public:
        ~AuraRenderable();
        AuraRenderable( ShaderProgramPtr program, BoidPtr boid );

    private:
        void do_draw();
        void do_animate( float time );

        void do_keyPressedEvent( sf::Event& e );

        bool m_display = true;
        BoidPtr m_boid;

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
};

typedef std::shared_ptr<AuraRenderable> AuraRenderablePtr;

#endif
