#ifndef BOID_RENDERABLE_HPP
#define BOID_RENDERABLE_HPP

#include <glm/glm.hpp>
#include "../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include "Boid.hpp"

/**
 * @class BoidRenderable
 * @brief Renderable for the boid. @see Renderable
 */
class BoidRenderable : public HierarchicalRenderable
{
    public:
        BoidRenderable(ShaderProgramPtr program, BoidPtr boid);
        ~BoidRenderable();
        void setMaterial(const MaterialPtr& material);

    private:
        void do_draw();
        void do_animate( float time );

        BoidPtr m_boid;

        std::vector< glm::vec3 > m_positions; ///< Positions of the boid
        std::vector< glm::vec4 > m_colors; ///< Colors of the boid
        std::vector< glm::vec3 > m_normals; ///< Normals of the boid
        std::vector< glm::vec2 > m_texCoords;

        unsigned int m_pBuffer; ///< Buffer for the position of the boid
        unsigned int m_cBuffer; ///< Buffer for the colors of the boid
        unsigned int m_nBuffer; ///< Buffer for the normals of the boid
        unsigned int m_tBuffer;
        unsigned int m_texId;

        MaterialPtr m_material; ///< Material of the boid
};

typedef std::shared_ptr<BoidRenderable> BoidRenderablePtr;

#endif