#ifndef VIEWER_HPP
#define VIEWER_HPP

/**@file
 * @brief Define the viewer.
 *
 * This file defines the Viewer class that manages the display window,
 * the renderables, the user input and the shader programs.
 */

#include "Renderable.hpp"
#include "HierarchicalRenderable.hpp"
#include "Camera.hpp"
#include "lighting/Light.hpp"
#include "TextEngine.hpp"
#include "FPSCounter.hpp"

#include "dynamics/Particle.hpp"

#include <unordered_set>
#include <memory>
#include <string>
#include <chrono>

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * \brief Manage the rendering and the interaction in a window.
 *
 * The Viewer class allows to visualize virtual contents and interact with it.
 * It is the key class of the the programs you will build in these practicals.
 */
class Viewer
{
public:
    /** We use the system_clock of std::chrono to compute durations.
     * chrono is a nice addition to the c++ std, go have a look there:
     * http://www.cplusplus.com/reference/chrono/
     */
    typedef std::chrono::system_clock clock;
    /** We use a double precision duration type to have a precise animation.
     */
    typedef std::chrono::duration<double> Duration;
    /** Based on the clock and the duration types, we define a time point type.
     */
    typedef std::chrono::time_point< clock, Duration > TimePoint;


    /**@name Construction / Destruction
     * @{*/
    /** @brief Destructor
     *
     * Instance destructor. */
    ~Viewer();

    /**@brief Constructor with window size
     *
     * Construct a new viewer that will display the scene in a window of
     * specified size.
     * \param width Width of the window in pixel.
     * \param height Height of the window in pixel.
     */
    Viewer(float width, float height);
    /**@}*/

    /** @name Shader Program management
     * We can make more evolved and useful shader program management. However, it
     * would be also more complex (to use and to understand) and maybe a little
     * restrictive for what you want to do in your project. Thus we kept the shader
     * program management of the Viewer quite simple:
     * \li add a shader program to manage
     * \li reload all managed shader program
     * @{
     */
    /**@brief Manage a shader program.
     *
     * Add a shader program to the list of managed programs.
     * @param program The shader program to manage.
     */
    void addShaderProgram( ShaderProgramPtr program );
    /**@brief Reload managed shader programs from its sources.
     *
     * Reload each managed shader program, to use possibly newer shader sources.
     * This is useful if you want to see the effects immediately of a modification
     * in the shader sources on the scene.
     *
     * \sa ShaderProgram::reload()
     */
    void reloadShaderPrograms();

    /**@name Main loop functions
     * @{*/
    /**@brief Check if the graphical application is still running.
     *
     * Tells if the graphical application is still running.
     * @return True if it is still running.
     */
    bool isRunning() const;
    /**@brief Display the scene on the windows.
     *
     * Display the scene stored in the framebuffer onto the window.
     */
    void display();
    /**\brief Draw the renderables.
     *
     * Iterate over all the renderables of \ref m_renderables and call their Renderable::draw() function.
     * For each renderable, the viewer will first bind its shader, send camera information to
     * the GPU, draw the renderable and unbind its shader.
     */
    void draw();

    /** \brief Animate the renderables.
     *
     * Iterate over all the renderable of m_renderables and call their function animate.
     */
    void animate();
    /** \brief handleEvent
     * Manage mouse, keyboards and window events.
     */
    void handleEvent();
    /**@}*/

    /**@name Utilities
     * @{*/
    /**
     * @brief Change the camera behavior.
     *
     * Switch the camera behavior from the Arcball mode to the Spaceship mode and vice-versa.
     */
    void changeCameraMode();

    /**
     * @brief Tells the viewer to ignore the camera mode and follow a particle
     */
    void followParticle(ParticlePtr particle);
    

    /**@brief Get the camera.
     *
     * Access to the camera used to render the scene in the viewer.
     * @return A reference to the viewer's camera. */
    Camera& getCamera();

    /**@brief Get the world coordinate of a window point.
     *
     * This function returns the world coordinate of a point given in the
     * window coordinate system. This is handy to find where the mouse is
     * in the scene. */
    glm::vec3 windowToWorld( const glm::vec3& windowCoordinate );

    /**@brief Get the window coordinate of a world point.
     *
     * This function returns the window coordinate of a point given in the
     * world coordinate system.
     */
    glm::vec3 worldToWindow( const glm::vec3& worldCoordinate );

    /**
     * \brief addRenderable
     *
     * Add a renderable to the set of renderabbles \ref m_renderables of the viewer.
     * \param r A renderable to add to \ref m_renderables.
     */
    void addRenderable( RenderablePtr r );

    /**
     * @brief Take a screen shot.
     *
     * Save a screenshot of the window in a PNG file in the directory containing the executable.
     */
    void takeScreenshot();
    /**@}*/

    /**@name Animation
     * @{
     */

    /**\brief Get the current animation time.
     *
     * Get the current animation time, since the beginning of the animation loop.
     * \return The current animation time.
     */
    float getTime();

    /**\brief Start the animation
     *
     * Start the animation loop by setting \ref m_animationIsStarted to true.
     */
    void startAnimation();

    /**\brief Stop the animation
     *
     * Stop the animation loop by setting \ref m_animationIsStarted to false.
     */
    void stopAnimation();

    /** \brief Reset the animation.
     *
     * Reset the animation time to the beginning.
     */
    void resetAnimation();

    /** \brief Set an animation loop.
     *
     * Activate or deactivate the loop on the animation. If animationLoop is set
     * to true, a \a loopDuration should be given.
     * \param animationLoop True if the animation should loop after \ref m_loopDuration.
     * \param loopDuration Set \ref m_loopDuration value. 0.0 is the default value.
     */
    void setAnimationLoop(bool animationLoop, float loopDuration=0.0);
    /**@}*/

    void setDirectionalLight(const DirectionalLightPtr& directionalLight);

    void addPointLight(const PointLightPtr& pointLight);

    void addSpotLight(const SpotLightPtr& spotLight);

    void displayText(std::string text, Viewer::Duration duration = std::chrono::seconds(3));

private:
    /**@brief Forbidden default constructor.
     *
     * By making it private, this constructor cannot be called. This is justified as we need to
     * know the initial display window size at construction.
     */
    Viewer();

    /**
     * \brief keyPressedEvent
     * Manage key pressing events.
     * \param e a SFML event.
     */
    void keyPressedEvent(sf::Event& e);

    /**\brief keyReleasedEvent
     * Manage key releasing events.
     * \param e a SFML event.
     */
    void keyReleasedEvent(sf::Event& e);

    /**
     * \brief mousePressEvent
     * Manage mouse pressing events.
     * \param e a SFML event.
     */
    void mousePressEvent(sf::Event& e);

    /**
     * \brief mouseReleaseEvent
     * Manage mouse releasing events.
     * \param e a SFML event.
     */
    void mouseReleaseEvent(sf::Event& e);

    /**
     * \brief mouseWheelEvent
     * Manage mouse wheeling event.
     * \param e a SFML event.
     */
    void mouseWheelEvent(sf::Event& e);

    /**
     * \brief mouseMoveEvent
     * Manage mouse motion event.
     * \param e a SFML event.
     */
    void mouseMoveEvent(sf::Event& e);


    Camera m_camera; /*!< Camera used to render the scene in the Viewer. */
    sf::RenderWindow m_window; /*!< Pointer to the render window. */
    std::unordered_set< RenderablePtr > m_renderables; /*!< Set of renderables that the viewer displays. */
    DirectionalLightPtr m_directionalLight; /*!< Pointer to a directional light. */
    std::vector<PointLightPtr> m_pointLights; /*!< Vector of pointer to the point lights. */
    std::vector<SpotLightPtr> m_spotLights; /*!< Vector of pointer to the spot lights. */


    std::unordered_set< ShaderProgramPtr > m_programs;

    TextEngine m_tengine; /*!< Engine to display textual information. */
    TimePoint m_modeInformationTextDisappearanceTime; /*!< Duration of appearance for textual information in seconds. */
    std::string m_modeInformationText; /*!< Textual information that will be displayed. */

    bool m_applicationRunning; /*!< Boolean that runs the main animation loop. Always true except when closing the application. */
    bool m_animationLoop; /*!< True if the animation loops after a given duration, \ref m_loopDuration. */
    bool m_animationIsStarted; /*!< True if the animation is running. False otherwise. */
    float m_loopDuration; /*!< Duration of the animation loop in seconds. */
    float m_simulationTime; /*!< Current simulation time in the animation loop. */
    TimePoint m_lastSimulationTimePoint; /*!< Date of the last simulation. */

    glm::vec3 m_currentMousePosition; /*!< Current mouse cursor coordinates normalized between [-1,1]. The z-value is set to 1. */
    glm::vec3 m_lastMousePosition; /*!< Previous mouse cursor coordinates normalized between [-1,1]. The z-value is set to 1. */

    unsigned int m_screenshotCounter; /*!< Number of screenshots since the beginning of the application. */

    FPSCounter m_fpsCounter; /*!< A framerate counter */
    bool m_helpDisplayed;

    /**@brief Hold important state of the keyboard.
     *
     * This class holds some state of the keyboard to be used to control
     * the camera in Camera::SPACESHIP_BEHAVIOR.
     */
    struct KeyboardState {
      bool forward;
      bool backward;
      bool left;
      bool right;
      bool slow;
      bool fast;

      glm::vec3 direction;
      float speed;

      KeyboardState();
    };

    KeyboardState keyboard; /*!< Help us to smoothly control the camera with the keyboard. */
    TimePoint m_lastEventHandleTime; /*!< Last time all input events were handled.*/

    /**
     * @brief Values to set if the camera of the viewer follow a particle
     */
    bool isFollowing = false;
    ParticlePtr m_particleToFollow = NULL;
    /**
     * @brief Values to limit the fps
     */
    double oneOverFps = 1./50.;
    double oldTime = -1.;
    
};

#endif
