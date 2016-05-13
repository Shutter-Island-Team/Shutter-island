#ifndef CAMERA_HPP
#define CAMERA_HPP

/**@file
 * @brief Define a camera object.
 *
 * Define a camera object to ease the manipulation of the view matrix by user
 * input such as the keyboard of the mouse.
 */

#include <glm/glm.hpp>
/**@brief Manage the Camera.
 *
 * We consider a camera to be defined by two 4x4 matrices:
 *  - the view matrix
 *  - the projection matrix.
 *
 * The view matrix defines the position and the orientation of the camera in the
 * world coordinates. It is represented as follow:
 * | Column 0 | Column 1 | Column 2 | Column 3 |
 * |----------|----------|----------|----------|
 * |    X0    |    X1    |    X2    |    P0    |
 * |    Y0    |    Y1    |    Y2    |    P1    |
 * |    Z0    |    Z1    |    Z2    |    P2    |
 * |    0     |    0     |    0     |    1     |
 * X=(X0,X1,X2), Y=(Y0,Y1,Y2) and Z=(Z0,Z1,Z2) represent the right, up, and
 * backward direction of the camera in the world coordinates. The point
 * P=(P0,P1,P2) is the position of the world origin in the camera frame
 * (X,Y,Z). Thus, the point (-dot(P,X), -dot(P,Y), -dot(P,Z)) is the position
 * of the camera in the world coordinates.
 *
 * The projection matrix defines the way the scene viewed by the camera is
 * transformed (projected) to a 2D image that will be displayed on screen. To
 * compute this matrix, we need ratio of the 2D image, the field of view and
 * the far and near cutting plane distances.
 */
class Camera
{
public:
    /**@brief Construction.
     *
     * Create a default camera located at (0,0,-5), looking at the position
     * (0,0,0), with an up vector equals to (0,1,0). The default camera
     * behavior is Camera::ARCBALL_BEHAVIOR. If such default behavior does not
     * suit you, feel free to change it in the implementation file.
     */
    Camera();

    /**@brief Destruction.
     *
     * Instance destruction.
     */
    ~Camera();

    /** @brief Animate the camera
     *
     * This function is currently empty, but you can define an animation
     * behavior here. This function is automatically called by the Viewer
     * @param time Current simulation time.
     */
    void animate( float time );

    /**
     * @name Camera View Matrix
     * Set of methods which interact with the camera view matrix. This matrix
     * which determines the camera frame in the world coordinates, i.e. the way
     * the camera is positioned and oriented.
     * @{
     */
    /**@brief Read access to the view matrix.
     *
     * Allow a read-only access to the view matrix, i.e. where the camera looks.
     * @return The view matrix
     */
    const glm::mat4& viewMatrix() const;

    /**@brief Write to the view matrix.
     *
     * Allow to write to the view matrix. This could be useful if the view
     * matrix is handled in another place than in the camera.
     * @param view The new view matrix. */
    void setViewMatrix(const glm::mat4& view);

    /**@brief Read access to the camera world position.
     *
     * Allow a read-only access to the camera position in world coordinates.
     * @return The camera position.
     */
    glm::vec3 getPosition() const;

    /**@brief Read access to the camera right direction.
     *
     * Allow a read-only access to the right direction of the camera in world
     * coordinates. This direction corresponds to the (+X) direction of the
     * camera local frame.
     *
     * @return The camera right direction
     */
    glm::vec3 getRight() const;

    /**@brief Read access to the camera up direction.
     *
     * Allow a read-only access to the up direction of the camera in world
     * coordinates. This direction corresponds to the (+Y) direction of the
     * camera local frame.
     *
     * @return The camera up direction.
     */
    glm::vec3 getUp() const;

    /**@brief Read access to the camera forward direction.
     *
     * Allow a read-only access to the forward direction of the camera in world
     * coordinates. This direction corresponds to the (-Z) direction of the
     * camera local frame, also known as the 'looking direction'.
     *
     * @return The camera forward direction.
     */
    glm::vec3 getForward() const;

    /**@brief Set the camera world position.
     *
     * Set the camera position in world coordinates.
     * @param pos New camera world position.
     */
    void setPosition( const glm::vec3& pos );

    /**@brief Set the camera right direction.
     *
     * Set  the camera right direction in world coordinates.
     * @param right New camera right axis in world coordinates.
     */
    void setRight( const glm::vec3& right );

    /**@brief Set the camera up direction.
     *
     * Set the camera up direction in world coordinates.
     * @param up New camera up axis in world coordinates.
     */
    void setUp( const glm::vec3& up );
    /**@brief Set the camera up direction.
     *
     * Set the camera forward (-Z) direction in world coordinates.
     * @param forward New camera forward direction in world coordinates.
     */
    void setForward( const glm::vec3& forward );
    ///@}


    /**
    * @name Camera Projection Matrix
    * Set of methods which interact with the projection matrix. This matrix
    * determine how a scene is projected on a 2D image that will be displayed
    * on screen.
    * @{
    */
    /**@brief Read access to the camera projection matrix.
     *
     * Allow a read-only access to the camera projection matrix. This matrix
     * defines how to go from the pixel coordinates to the screen coordinates.
     *
     * @return The projection matrix.
     */
    const glm::mat4& projectionMatrix() const;

    /**@brief Write to the projection matrix of the camera.
     *
     * Allow to write to the projection matrix of the camera.
     * @param projection The new projection matrix used by this camera. */
    void setProjectionMatrix(const glm::mat4& projection);

    /**@brief Get the camera field of view
     *
     * Get the field of view of the camera, also known as the camera angle.
     * @return The camera fov.
     */
    float fov() const;

    /**@brief Get the camera ratio.
     *
     * The camera ratio is the length ratio of the image taken by this camera. This
     * ratio is meant to be equal to the width of the display window divided by its
     * height. This ratio is also know as the aspect ratio.
     *
     * @return The length ratio of the image taken by this camera.
     */
    float ratio() const;

    /**@brief Get the near clipping plane.
     *
     * Get the near clipping plane: anything that is closer to the camera than this
     * will be removed from the rendered image.
     *
     * @return The near clipping plane distance.
     */
    float znear() const;

    /**@brief Get the far clipping plane.
     *
     * Get the far clipping plane: anything that is farther from the camera than this
     * plane will be removed from the rendered image.
     *
     * @return The far clipping plane distance.
     */
    float zfar() const;

    /**@brief Define the field of view.
     *
     * Set the camera field of view. We consider 1.04 is a good start to find a
     * custom fov.
     * @param v The new field of view.
     */
    void setFov( const float& v );

    /**@brief Define the aspect ratio.
     *
     * Set the camera aspect ratio. Generally, this is done at each window resize.
     * The Viewer automatically call this method on its camera when its window is
     * resized.
     * @param v The new aspect ratio (width / height)
     */
    void setRatio( const float& v );

    /**@brief Define the far clipping plane.
     *
     * Set the far clipping plane at the specified distance.
     * @param v The far clipping plane distance.
     */
    void setZfar( const float& v );

    /**@brief Define the near clipping plane.
     *
     * Set the near clipping plane at the specified distance.
     * @param v The near clipping plane distance.
     */
    void setZnear( const float& v );
    ///@}


    /** @brief Reaction to mouse displacement.
     *
     * This enumeration specifies the different kind of reaction to a mouse displacement.
     */
    enum CAMERA_MOUSE_BEHAVIOR {
      /** The view matrix is modified to turn the camera around the world origin.*/
      ARCBALL_BEHAVIOR,
      /** The view matrix is modified to point its 'nose' in the direction pointed by the mouse. */
      SPACESHIP_BEHAVIOR
    };

    /** @name Mouse Control
     * Control the camera with the mouse.
     * @{
     */
    /**@brief Get the mouse behavior.
     *
     * Access to the mouse behavior currently used by the update( dx, dy ) function.
     * @return The mouse behavior.
     */
    CAMERA_MOUSE_BEHAVIOR getMouseBehavior() const;

    /**@brief Set the mouse behavior.
     *
     * Set the new mouse behavior of the camera, that will be used by the update( dx, dy ) function.
     * @param v The new behavior.
    */
    void setMouseBehavior( const CAMERA_MOUSE_BEHAVIOR& v );
    /**@brief Update the camera view in reaction to a mouse displacement.
     *
     * Change the camera view matrix according to the current mouse behavior.
     * @param dx Mouse x displacement.
     * @param dy Mouse y displacement.
     */
    void update( float dx, float dy );
    ///@}
private:

    /** @name Private members */
    CAMERA_MOUSE_BEHAVIOR m_mouseBehavior;
    float m_fov;
    float m_ratio;
    float m_znear;
    float m_zfar;
    glm::mat4 m_view;
    glm::mat4 m_projection;
};

#endif
