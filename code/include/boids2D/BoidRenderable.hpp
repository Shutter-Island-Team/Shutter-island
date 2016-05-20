#ifndef BOID_RENDERABLE_HPP
#define BOID_RENDERABLE_HPP

#include <glm/glm.hpp>

#include "../HierarchicalRenderable.hpp"
#include "Boid.hpp"

class BoidRenderable : public HierarchicalRenderable
{
    public:
        BoidRenderable( ShaderProgramPtr program, BoidPtr boid );
        ~BoidRenderable();

        void mousePressEvent(sf::Event& e);

    private:
        void do_draw();
        void do_animate( float time );

        BoidPtr boid;

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
};

typedef std::shared_ptr<BoidRenderable> BoidRenderablePtr;

#endif