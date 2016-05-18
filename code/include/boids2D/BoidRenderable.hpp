#ifndef BOID_RENDERABLE_HPP
#define BOID_RENDERABLE_HPP

#include <glm/glm.hpp>

#include "../Renderable.hpp"
#include "Boid.hpp"

class BoidRenderable : public Renderable
{
    public:
        ~BoidRenderable();
        BoidRenderable();
        BoidRenderable( ShaderProgramPtr program, Boid boid );

        void do_keyPressedEvent(sf::Event& e);

    private:
        void do_draw();
        void do_animate( float time );

        Boid boid;

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
};

typedef std::shared_ptr<BoidRenderable> BoidRenderablePtr;

#endif