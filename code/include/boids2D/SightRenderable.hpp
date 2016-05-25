#ifndef CUBE_RENDERABLE_HPP
#define CUBE_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "MovableBoid.hpp"
#include <vector>
#include <glm/glm.hpp>

/**
 * @class SightRenderable 
 * @brief Renderable for a cone of vision of a boid
 */
class SightRenderable : public HierarchicalRenderable
{
 public:
  /**
   * @brief     Constructor of SightRenderable
   * @param[in] program Shader of the Renderable
   * @param[in] boid    Boid associated with the renderable
   */
  SightRenderable( ShaderProgramPtr program, MovableBoidPtr boid );

  /**
   * @brief Destructor of SightRenderable
   */
  ~SightRenderable();

 private:
  std::vector< glm::vec3 > m_positions; ///< Vector of the positions
  std::vector< glm::vec4 > m_colors; ///< Vector of the colors
  std::vector< glm::vec3 > m_normals; ///< Vector of the normals

  MovableBoidPtr m_boid; ///< Link to the boid
  bool m_display; ///< Boolean to check if the renderable needs to be displayed

  unsigned int m_pBuffer; ///< Buffer of the position
  unsigned int m_cBuffer; ///< Buffer of the color
  unsigned int m_nBuffer; ///< Buffer of the normal

  /**
   * @brief Implementation of do_draw to draw the renderable
   */
  void do_draw();

  /**
   * @brief Implementation of do_animate. Does nothing
   * @param[in] time Time since the beginning of the programm
   */
  void do_animate( float time );

  /**
   * @brief       Implementation of virtual function. Press 'P' to switch
   *              from invisible to visible
   * @param[in]   e The event received
   */
  void do_keyPressedEvent( sf::Event& e );
};

typedef std::shared_ptr<SightRenderable> SightRenderablePtr;

#endif