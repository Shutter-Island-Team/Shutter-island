#ifndef KART_CHASSIS_RENDERABLE_HPP
#define KART_CHASSIS_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "Particle.hpp"
#include "StickParticle.hpp"

#include <vector>
#include <glm/glm.hpp>


/**
 * @brief Render a chassis of a kart : Nothing to do for now
 */
class KartChassisRenderable : public HierarchicalRenderable
{
public:
    ~KartChassisRenderable();
    KartChassisRenderable( ShaderProgramPtr program, StickParticlePtr particle );
    void setMaterial(const MaterialPtr& material);


private:
    void do_draw();
    void do_animate( float time );

    StickParticlePtr m_particle;

    // None of the followings is used
    
    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
    MaterialPtr m_material;
};

typedef std::shared_ptr<KartChassisRenderable> KartChassisRenderablePtr;

#endif

