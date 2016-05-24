#ifndef BOID_RENDERABLE_HPP
#define BOID_RENDERABLE_HPP

#include <glm/glm.hpp>
#include "../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include "Boid.hpp"

class BoidRenderable : public HierarchicalRenderable
{
    public:
        BoidRenderable( ShaderProgramPtr program, BoidPtr boid);
        ~BoidRenderable();
        void setMaterial(const MaterialPtr& material);

        void mousePressEvent(sf::Event& e);

    private:
        void do_draw();
        void do_animate( float time );

        BoidPtr m_boid;

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;
        std::vector< glm::vec2 > m_texCoords;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
        unsigned int m_tBuffer;
        unsigned int m_texId;

        MaterialPtr m_material;
};

typedef std::shared_ptr<BoidRenderable> BoidRenderablePtr;

#endif