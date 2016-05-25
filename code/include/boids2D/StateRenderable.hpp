#ifndef State_RENDERABLE_HPP
#define State_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "MovableBoid.hpp"
#include "StateType.hpp"
#include <vector>
#include <glm/glm.hpp>

class StateRenderable : public HierarchicalRenderable
{
 public:
  /**
   * @brief       Constructor of StateRenderable
   * @param[in]   program Shader of the Renderable
   * @param[in]   boid    Boid associated with the renderable
   */
  StateRenderable( ShaderProgramPtr program, MovableBoidPtr boid );
  /**
   * @brief Destructor of StateRenderable
   */
  ~StateRenderable();

 private:
  bool m_display; ///< Boolean to check if the renderable needs to be displayed
  MovableBoidPtr m_boid; ///< Link to the boid
  StateType m_savedState; ///< Local known state. Useful to detect if
                          ///< a state of a boid has changed

  std::vector< glm::vec3 > m_positions; ///< Vector of the positions
  std::vector< glm::vec4 > m_colors; ///< Vector of the colors
  std::vector< glm::vec3 > m_normals; ///< Vector of the normals

  unsigned int m_pBuffer; ///< Buffer of the position
  unsigned int m_cBuffer; ///< Buffer of the color
  unsigned int m_nBuffer; ///< Buffer of the normal

  /**
   * @brief Implementation of do_draw to draw the renderable
   */
  void do_draw();

  /**
   * @brief     Implementation of do_animate. Does nothing
   * @param[in] time Time since the beginning of the programm
   */
  void do_animate( float time );

  /**
   * @brief       Implementation of virtual function. Press 'P' to switch
   *              from invisible to visible
   * @param[in]   e The event received
   */
  void do_keyPressedEvent( sf::Event& e );

  /**
   * @brief   return the color associated with the saved state
   * @return  the color associated with the saved state
   */
  glm::vec4 getColorFromState();
};

typedef std::shared_ptr<StateRenderable> StateRenderablePtr;

#endif
