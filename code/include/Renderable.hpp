#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP
/**@file
 *@brief Define the renderable interface.
 *
 * The Renderable class is the base class for all the 3D objects that will
 * be rendered in the practicals.
 */

#include <glm/glm.hpp>
#include <unordered_set>
#include <memory>
#include <vector>

#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include <SFML/Graphics.hpp>

/* Forward declaration of the Viewer class in order to store a pointer to a
 * renderable's viewer and to define this class as a friend of Renderable.
 */
class Viewer;

/**
 * @brief Renderable interface.
 *
 * A renderable is a concept often used in graphic applications. It can wear many
 * names, such as drawable in the Android Framework
 * http://developer.android.com/guide/topics/resources/drawable-resource.html and
 * in the SFML library http://www.sfml-dev.org/documentation/2.0/classsf_1_1Drawable.php,
 * or Renderable in the OGRE Engine
 * http://www.ogre3d.org/docs/api/1.9/class_ogre_1_1_renderable.html. There are other
 * commonly used concepts such as animable. Instead of building a wide hierarchy
 * of concepts, we chose to merge all the necessary concepts of the objects that
 * will compose our scenes into the Renderable class.
 *
 * This class is an interface we need to implement to create 3D objects. In
 * particular, we must define the pure virtual member functions do_draw() and
 * do_animate( float time ). The Viewer class can handle any non abstract derived
 * class to display it for you, animate it or to send interaction events to it.
 *
 * If you want the Viewer managing your renderables to automatically set the view
 * and the projection matrices for you, remember to name the corresponding uniform
 * matrices as follow in your shaders:
 * - \c mat4 \c viewMat, for the view matrix
 * - \c mat4 \c projMat, for the projection matrix
 *
 * \note As this class use virtuality, here are some words about the subject to
 * ease your learning of c++ as well as learning computer graphics. This note is
 * taken from a nice article available at http://www.gotw.ca/publications/mill18.htm.
 * Prefer to make base class virtual functions private (or protected if you
 * really must). This separates the concerns of interface and implementation,
 * which stabilizes interfaces and makes implementation decisions easier to
 * change and refactor later. Here are some guidelines:
 * \li #1 Prefer to make interfaces nonvirtual, using Template Method.
 * \li #2 Prefer to make virtual functions private.
 * \li #3 Only if derived classes need to invoke the base implementation of a
 * virtual function, make the virtual function protected.
 * \li #4 A base class destructor should be either public and virtual, or
 * protected and nonvirtual. In the first case, you can do something like
 * <tt> delete basePtr</tt>, while in the second case, you can not.
 *
 * \sa HierarchicalRenderable
 */
class Renderable
{
public:
    /** @name Construction/Destruction of renderable instances.*/
    /** @brief Destructor.
     *
     * Some smart pointers to this base class could be stored, such as in
     * Viewer. As such, the destructor needs to be public and virtual (guideline
     * #4).
     */
    virtual ~Renderable();

    /** @brief Renderable program constructor.
     *
     * Initialize the model matrix \ref m_model to identity.
     * Set the shader program \ref m_shaderProgram to \a program.
     * \param program Shader program to use to draw this.
     * \sa ShaderProgram
     */
    Renderable(ShaderProgramPtr program);

    /** @name Public interface */
    /** @brief Set the model matrix.
     *
     * Define the model matrix, that is the way this object is scaled, oriented
     * and placed in world space.
     * @param model The new model matrix.
     */
    void setModelMatrix( const glm::mat4& model );

    /**@brief Get the model matrix.
     *
     * Allow a read-only access to the model matrix.
     * @return The model matrix.
     */
    const glm::mat4& getModelMatrix() const;

    /**@brief Change the shader program.
     *
     * Set a new shader program to use for the rendering.
     * @param prog The new shader program.
     * \sa ShaderProgram
     */
    void setShaderProgram( ShaderProgramPtr prog );

    /** @name Public interface used by Viewer.
     * Those functions are automatically called by the Viewer on added Renderable.
     * You are not expected to call them directly. */
    /** \brief Bind the shader program on the GPU.
     *
     * Modify the OpenGL state such that the next drawing command use the
     * program shader of this renderable to render the primitives.
     */
    void bindShaderProgram();

    /**\brief Unbind the shader program of the GPU.
     *
     * Change the OpenGL state such that the next drawing command does not use
     * any shader program.
     */
    void unbindShaderProgram();
    /**
     * \brief Get the location of the projection matrix in the shader program.
     *
     * Get the uniform identifier of the projection matrix for the shader program
     * of this renderable. This uniform should be named "projMat" in your shader
     * to be automatically set by the Viewer.
     *
     * \return the location of the projection matrix uniform or -1 if the
     * associated shader program does not use such an uniform ("projMat").
     */
    int projectionLocation();

    /** \brief Get the location of the view matrix in the shader program.
     *
     * Get the uniform identifier of the view matrix for the shader program of
     * this renderable. This uniform should be named "viewMat" in your shader
     * to be automatically set by the Viewer.
     * \return the location of the view matrix uniform or -1 if the associated
     * shader program does not use such an uniform ("viewMat").
     */
    int viewLocation();
    /** \brief Draw this renderable.
     *
     * This function calls the private pure virtual function <tt> do_draw() </tt>
     * (guidelines #1 and #2). When this function is called by the Viewer,
     * the shader program is already binded, the view and projection matrices are
     * already set.
     */
    void draw();

    /** \brief Animate this renderable.
     *
     * This function calls the private pure virtual function <tt> do_animate(time) </tt>
     * (guidelines #1 and #2). The simulation time equals zero when the animation is
     * started (see Viewer::startAnimation()).
     * \param time Current simulation time.
     */
    virtual void animate( float time );

    /**
     * \brief Handle a key pressed event.
     *
     * React to a key pressed.
     * \param e The key pressed event.
     */
    void keyPressedEvent(sf::Event& e);

    /** \brief Handle a key released event.
     *
     * React to a key released.
     * \param e The key released event.
     */
    void keyReleasedEvent(sf::Event& e);

    /** \brief Handle a mouse pressed event.
     *
     * React to a mouse pressed.
     * \param e The mouse pressed event.
     */
    void mousePressEvent(sf::Event& e);

    /** \brief Handle a mouse released event.
     *
     * React to a mouse released.
     * \param e The mouse release event.
     */
    void mouseReleaseEvent(sf::Event& e);

    /** \brief Handle a mouse wheel event.
     *
     * React to a mouse wheel change.
     * \param e The mouse wheel event.
     */
    void mouseWheelEvent(sf::Event& e);

    /** \brief Handle a mouse move event.
     *
     * React to a mouse displacement.
     * \param e The mouse displacement event.
     */
    void mouseMoveEvent(sf::Event& e);

    ShaderProgramPtr getShaderProgram() const;

    void displayTextInViewer(std::string text) const;


private:
    /** @name Private viewer interface.
     * According to guideline #1, we have used the Template method. Now we are
     * using guideline #2 for virtual methods.*/
    /** \brief Draw virtual function.
     *
     * Implementation to draw this renderable.
     */
    virtual void do_draw() = 0;

    /** \brief Animate virtual function.
     *
     * Implementation to animate this renderable.
     * \param time The current simulation time.
     */
    virtual void do_animate( float time ) = 0;
    /**
     * \brief Handle a key pressed event.
     *
     * Implementation to react to a key pressed.
     * \param e The key pressed event.
     */
    virtual void do_keyPressedEvent(sf::Event& e);

    /** \brief Handle a key released event.
     *
     * Implementation to react to a key released.
     * \param e The key released event.
     */
    virtual void do_keyReleasedEvent(sf::Event& e);

    /** \brief Handle a mouse pressed event.
     *
     * Implementation to react to a mouse pressed.
     * \param e The mouse pressed event.
     */
    virtual void do_mousePressEvent(sf::Event& e);

    /** \brief Handle a mouse released event.
     *
     * Implementation to react to a mouse released.
     * \param e The mouse release event.
     */
    virtual void do_mouseReleaseEvent(sf::Event& e);

    /** \brief Handle a mouse wheel event.
     *
     * Implementation to react to a mouse wheel change.
     * \param e The mouse wheel event.
     */
    virtual void do_mouseWheelEvent(sf::Event& e);

    /** \brief Handle a mouse move event.
     *
     * Implementation to react to a mouse displacement.
     * \param e The mouse displacement event.
     */
    virtual void do_mouseMoveEvent(sf::Event& e);


    /** @name Private interface for Renderable sub classes.
      * Those functions are meant to be overridden in subclassed of Renderable,
      * in order to have additional operations done before and after drawing
      * and animating.
      * \sa HierarchicalRenderable.*/
    /**@brief Perform operation before drawing a renderable.
     *
     * Override this function to perform additional operations before calling
     * do_draw() of the concrete renderable class.
     */
    virtual void beforeDraw();
    /**@brief Perform operation after drawing a renderable.
     *
     * Override this function to perform additional operations after calling
     * do_draw() of the concrete renderable class.
     */
    virtual void afterDraw();
    /**@brief Perform operation before animating a renderable.
     *
     * Override this function to perform additional operations before calling
     * do_animate() of the concrete renderable class.
     * @param time The current simulation time.
     */
    virtual void beforeAnimate( float time );
    /**@brief Perform operation after animating a renderable.
     *
     * Override this function to perform additional operations after calling
     * do_animate() of the concrete renderable class.
     * @param time The current simulation time.
     */
    virtual void afterAnimate( float time );

    Viewer* getViewer() const;


   

protected:
    /** @name Protected members.
     * We want those members to be accessible in the derived classes.
     */
    ShaderProgramPtr m_shaderProgram; /*!< Shader program of the renderable. */
    glm::mat4 m_model; /*!< Model matrix of the renderable. */

    /* The viewer is declared as a friend to be able to set the field m_viewer
     * when a Renderable is added to a viewer. If we want to get rid of the
     * the friend declaration, we have two solutions:
     *
     * - passing the viewer as a constructor parameter. Then the constructor
     * could had the instance itself to the viewer. We chose to not do that
     * this year as it would mean to change the code students wrote in
     * previous practicals.
     *
     * - having a method setViewer() publicly defined. However, this means
     * that we could set a null viewer or to set a viewer that is not
     * managing the Renderable. We ignored this solution.
     */
    friend Viewer;
    Viewer* m_viewer; /*!< Viewer instance that manage this renderable */

};

typedef std::shared_ptr<Renderable> RenderablePtr; /*!< Typedef for smart pointer to renderable.*/

#endif
