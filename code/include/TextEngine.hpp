#ifndef TEXTENGINE_HPP_
#define TEXTENGINE_HPP_

/** @file
 * @brief Define a text engine.
 *
 * This file define an object that can display text on screen thanks to OpenGL.
 * That object is TextEngine.
 */

# include <string>
# include <glm/glm.hpp>
# include "ShaderProgram.hpp"

/**
 * Forward declaration of the texture atlas class. This allows us to store
 * pointer to the texture atlas in the TextEngine class without defining the
 * texture atlas class. This type is kept private as there is no need for the
 * user of the text engine to know its detail.
 */
class TextAtlas;
/** @brief Print text on the screen.
 *
 * We decided to use a simple texture atlas technique. The visible
 * characters of a font for a particular size are loaded in an openGL
 * texture. For each character of the message to print, a quad is
 * textured with the subtexture corresponding to the character in the atlas.
 * It is not required to understand the whole process. However, we chose this
 * technique instead of more evolved one since this one can be understood
 * with the content of the course.
 *
 * We built this class by adapting the two following resources:
 * https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01
 * http://learnopengl.com/#!In-Practice/Text-Rendering
 *
 * Limitations:
 *
 *  - display only US ASCII characters
 *
 *  - when the same message is displayed the same way for more than one
 *  frame, it is far from efficient: the same data is computed and send
 *  over and over for the same result. This becomes more noticeable when
 *  there are a lot of text displayed on the screen for some seconds. To
 *  address this issue, it is possible to render the whole message to a
 *  texture and then display it on screen thanks to a quad.
 */
class TextEngine
{
public:
  /** @brief Construction
   *
   * Build a text engine using an monospace font with a height of 24 pixels.
   */
  TextEngine();

  /** @brief Instance destruction.
   *
   * Destroy the atlas, which in turn, release the font texture from the GPU.
   */
  ~TextEngine();

  /** @brief Initialize the font engine.
   *
   * The text engine needs a properly set OpenGL context to be fully initialized.
   * The initialization does not take place in the constructor such that we can
   * control when OpenGL commands will be issued, to make them happen after the
   * context creation.
   *
   * We stored a text engine instance inside the Viewer class. The viewer will
   * automatically call this function for you. Thus, except if you really want
   * you own text engine instance, you do not have to worry about this function.
   */
  void init();

  /** @brief Notify the engine a change in the window size.
   *
   * When the display window changed its size, it should be notified to the
   * text engine. Indeed, those dimensions are necessary to compute the
   * projection matrix, i.e. how to pass from the pixel coordinates to the
   * screen coordinates.
   * @param width Width in pixels of the window we are rendering to
   * @param height Height in pixels of the window we are rendering to
   */
  void setWindowDimensions( float width, float height );

  /** @brief Render a text message on the screen.
   *
   * Render a message on the screen with a specified color, position and size.
   * The default size (for a scale of 1.0f) is 24 pixels. Thus the size with
   * a scale of 2.0 would be 48 pixels.
   *
   * This functions handle multiple line message, with a separation between
   * lines equals to 25% of the maximum character height. Also, please note
   * that the line restarts at the vertical position of the first message
   * character, that is pixelCoordinates.x.
   * @param msg The message to be displayed on the screen
   * @param pixelCoordinates Position of the first message character
   * @param textColor Color of the message
   * @param scale The size of the message would be 24 * scale pixels
   */
  void render( const std::string& msg, const glm::vec2& pixelCoordinates,
               const glm::vec3& textColor, float scale = 1.0f );

private:
  /** Holds a texture that contains all the US-ASCII visible glyphs of a
   * particular font rendered with a particular character height. There are
   * also all necessary information to compute the vertex and texture
   * coordinates for each character. We store a pointer here as this type
   * is not required to be known in order to use the text engine.
   */
  TextAtlas* m_atlas;
  /** How to project pixel coordinates onto the screen.
   */
  glm::mat4 m_projection;
  /** Process the vertices and a texture to display a text on the screen.
   */
  ShaderProgram m_program;
  /** Name of the buffer containing the vertex info
   */
  unsigned int m_vboVertices;
  /** Where to bind the vertex info VBO on the GPU to be used by the shader
   * program.
   */
  int m_vertexLocation;
  /** Where to send the values of the projection matrix on the GPU to be
   * used by the shader program.
   */
  int m_projectionLocation;
  /** Where to send the text color on the GPU to be used by the shader program.
   */
  int m_colorLocation;
  /** Where to send the texture unit number on the GPU to be used by the shader
   * program.
   */
  int m_samplerLocation;
};

#endif
