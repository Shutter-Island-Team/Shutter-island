#ifndef KART_WHEEL_RENDERABLE_HPP
#define KART_WHEEL_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "Particle.hpp"
#include "RoundedParticle.hpp"

#include <vector>
#include <glm/glm.hpp>

/**@brief Render a particle to the screen.
 *
 * Render a particle to the screen. Since a particle is modeled by
 * a ball, this renderable simply render the corresponding ball. If
 * you have more than one renderable, have a look to ParticleListRenderable.
 */
class KartWheelRenderable : public HierarchicalRenderable
{
    public:
        ~KartWheelRenderable();
        /**@brief Build a particle renderable.
         *
         * Build a renderable to render a particle.
         * @param program The shader program used to render the particle.
         * @param particle The particle to render.
         */
    KartWheelRenderable( ShaderProgramPtr program, RoundedParticlePtr particle, int nb );
        void setMaterial(const MaterialPtr& material);



    private:
        void do_draw();
        void do_animate( float time );

        RoundedParticlePtr m_particle;

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
        MaterialPtr m_material;
};

typedef std::shared_ptr<KartWheelRenderable> KartWheelRenderablePtr;

#endif //KART_RENDERABLE_HPP
