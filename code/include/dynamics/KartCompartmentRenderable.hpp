#ifndef KART_COMPARTMENT_RENDERABLE_HPP
#define KART_COMPARTMENT_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "Particle.hpp"
#include "RoundedParticle.hpp"
#include "BoxParticle.hpp"

#include <vector>
#include <glm/glm.hpp>

/**
 * @brief Render the body of a kart
 */
class KartCompartmentRenderable : public HierarchicalRenderable
{
public:
    ~KartCompartmentRenderable();
    //KartCompartmentRenderable( ShaderProgramPtr program, RoundedParticlePtr particle );
    KartCompartmentRenderable( ShaderProgramPtr program, BoxParticlePtr particle );
    void setMaterial(const MaterialPtr& material);



private:
    void do_draw();
    void do_animate( float time );

    //RoundedParticlePtr m_particle;
    BoxParticlePtr m_particle;

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
    MaterialPtr m_material;
};

typedef std::shared_ptr<KartCompartmentRenderable> KartCompartmentRenderablePtr;

#endif //KART_COMPARTMENT_RENDERABLE_HPP
