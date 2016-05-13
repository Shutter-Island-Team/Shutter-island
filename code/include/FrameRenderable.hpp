#ifndef FRAME_RENDERABLE_HPP
#define FRAME_RENDERABLE_HPP
/**@file
 * @brief Define a class to render the world frame.
 *
 * This file defines the class FrameRenderable to render the world frame.
 */
#include "Renderable.hpp"
#include <vector>
#include <glm/glm.hpp>

/**@brief Render the world frame.
 *
 * This class is the first renderable you will use (see the Tutorial 03 of
 * practical 01). It will render the world frame on the screen: a red line
 * for the X axis, a green line for the Y axis and a blue line for the Z
 * axis. Have a look at the source: we documented it to help you to understand
 * the details.
 */

// A renderable must implement the Renderable interface, as such we make this
// class inherit publicly from Renderable.
class FrameRenderable : public Renderable
{
// Only the constructor and the destructor are meant to be called outside of
// the class. This is why they constitute the only public methods.
public :
    /**@brief Destructor.
     *
     * Instance destructor.
     */
    ~FrameRenderable();
    /**@brief Constructor.
     *
     * Create a new instance that will rendered thanks to the given shader program.
     * @param shaderProgram The shader program to use to render this frame renderable.
     */
    FrameRenderable(ShaderProgramPtr shaderProgram);

// Define the private section: member and functions only accessible to the class itself
private:
    /**@brief Forbidden default constructor.
     *
     * By putting the default constructor in the private section, we make it
     * forbidden (since it is private, no other class can call it). We do not
     * need to provide an implementation as the compiler will never need it.*/
    FrameRenderable();

    /**@brief Draw this renderable.
     *
     * The implementation of the draw() interface of Renderable. This is where
     * drawing commands are issued.
     */
    void do_draw();

    /**@brief Animate this renderable.
     *
     * The implementation of the animate( float time ) or Renderable. This does
     * nothing as we do not need to animate it.
     * @param time The current simulation time.
     */
    void do_animate( float time );


    /**@brief Initialize the vertex attributes of the frame.
     *
     * This function intializes the geometry and the color of the frame. We put
     * it in its own method help you to understand what is done in the constructor.
     */
    void initAttributes();

    /**@brief Position attribute of the vertices.
     *
     * Store the position attribute of the vertices. Notice that all the instances,
     * those positions would be the same. Thus, you could make this data member
     * static: shared by all instances of the class. We decided to not make it
     * static to help people that are new to c++.
     */
    /**@brief Color attribute of the vertices.
     *
     * Store the color attribute of the vertices. Notice that all the instances,
     * those colors would be the same. Thus, you could make this data member
     * static: shared by all instances of the class. We decided to not make it
     * static to help people that are new to c++.
     */
    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;

    /**@brief Location of the position attribute buffer.
     *
     * Store the location (index) of the position attribute buffer on the shader
     * program used to render the instance. If all instances are rendered by the
     * same shader program, you could make this location static and set this
     * location when the first instance is created. We decided to not make it
     * static as it would be harder to understand and it is quite constraining:
     * if you want to use more than shader program to render frame renderable
     * instances, this will lead to bugs tricky to understand for beginners.
     */
    unsigned int m_pBuffer;

    /**@brief Location of the color attribute buffer.
     *
     * Store the location (index) of the color attribute buffer on the shader
     * program used to render the instance. If all instances are rendered by the
     * same shader program, you could make this location static and set this
     * location when the first instance is created. We decided to not make it
     * static as it would be harder to understand and it is quite constraining:
     * if you want to use more than shader program to render frame renderable
     * instances, this will lead to bugs tricky to understand for beginners.
     */
    unsigned int m_cBuffer;

    /* Note: We could store all the buffers at the same place.
     *
     * enum { POSITION_BUFFER, COLOR_BUFFER, NUMBER_OF_BUFFERS };
     * unsigned int m_buffers[ NUMBER_OF_BUFFERS ];
     *
     * Then, when we generate those buffers:
     * glGenBuffers( NUMBER_OF_BUFFERS, m_buffers );
     *
     * And at destruction:
     * glDeleteBuffers( NUMBER_OF_BUFFERS, m_buffers );
     */
};

typedef std::shared_ptr<FrameRenderable> FrameRenderablePtr;

#endif
