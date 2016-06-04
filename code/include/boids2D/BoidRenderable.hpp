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
  /**
   * @brief       Constructor of BoidRenderable
   * @param[in]   program Shader of the Renderable
   * @param[in]   boid    Boid associated with the renderable
   */
   BoidRenderable(ShaderProgramPtr program, BoidPtr boid);

   /**
    * @brief Destructor of the boid renderable
    */
   ~BoidRenderable();

   /**
    * @brief     Setter for the material
    * @param[in] material The material to set
    */
   void setMaterial(const MaterialPtr& material);

 private:
  /**
   * @brief Implementation of do_draw to draw the renderable
   */
  void do_draw();
  
  /**
   * @brief Implementation of do_animate. Does nothing
   * @param[in] time Time since the beginning of the programm
   */
  void do_animate( float time );

  unsigned int m_pBuffer; ///< Buffer for the position of the boid
  unsigned int m_cBuffer; ///< Buffer for the colors of the boid
  unsigned int m_nBuffer; ///< Buffer for the normals of the boid
  unsigned int m_tBuffer;
  unsigned int m_texId;

  BoidPtr m_boid; ///< The boid linked to the renderable

  std::vector< glm::vec3 > m_positions; ///< Positions of the boid
  std::vector< glm::vec4 > m_colors; ///< Colors of the boid
  std::vector< glm::vec3 > m_normals; ///< Normals of the boid
  std::vector< glm::vec2 > m_texCoords;

  MaterialPtr m_material; ///< Material of the boid
};

typedef std::shared_ptr<BoidRenderable> BoidRenderablePtr;

#endif