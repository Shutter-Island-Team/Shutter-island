#ifndef CUSTOMIZED_KEYFRAME_HPP
#define CUSTOMIZED_KEYFRAME_HPP


#if 0
/**@file
 * @brief Define a key frame
 *
 * This file defines the class CustomizedKeyFrame for the animation practical.
 */

#include "KeyFrame.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>

/**@brief A key frame.
 *
 * This class defines a particular state composed of an orientation, a scale,
 * a translation and a time, named a key frame. This state can be then
 * interpolated between two frames. This way, we can define the state of a
 * Renderable at two particular key frames and generate additional states
 * for in between times. This is used to build a Renderable model matrix for
 * each time.
 *
 * \note No sheering state is managed by this class.
 */
class CustomizedKeyFrame : public KeyFrame
{
public:
    /**@brief Construct a key frame
     *
     * Construct a key frame from the specified time, orientation, scale and
     * translation.
     * @param time Time of this key frame (in second).
     * @param orientation Orientation of this key frame.
     * @param scale Scale of this key frame.
     * @param translation Translation of this key frame.
     */
    CustomizedKeyFrame(
        float time,
        const glm::quat& orientation = glm::quat{},
        const glm::vec3& scale = glm::vec3{1,1,1},
        const glm::vec3& translation = glm::vec3{} );

    /**@brief Copy constructor
     *
     * Construct a copy of a key frame.
     * @param keyFrame The key frame to copy into this.
     */
    CustomizedKeyFrame( const CustomizedKeyFrame& keyFrame);

    /**@brief Construct a key frame.
     *
     * Construct a key frame from a time and a transformation matrix. Be
     * careful: the transformation needs to contains only translation,
     * rotation and scaling (no sheering allowed).
     */
    CustomizedKeyFrame( float time, const glm::mat4& transformation );

    /**@brief Interpolate a transformation matrix between two key frames.
     *
     * Interpolate a transformation matrix at a specified time thanks to two
     * key frames \c k1 and \c k2. If the specified time is smaller than the
     * one of \c k1, the transformation matrix of k1 is returned. Similarly,
     * if the specified time is smaller than the one of \c k2, the
     * transformation matrix of \c k2 is returned.
     *
     * The interpolation is made component by component, i.e. the translation
     * is linearly interpolated, then the scale and the orientation. Those
     * interpolations are then combined to form a transformation matrix. As the
     * orientation is represented by a quaternion, every goes smoothly. We just
     * need to normalize the interpolated quaternion before combining the
     * different parts into a transformation matrix. If you want to understand
     * the underlying details, have a look to this article
     * http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
     */
    static KeyFramePtr interpolate(const KeyFramePtr& k1, const KeyFramePtr& k2, float time );

    /**@brief Get the time point of this key frame.
     *
     * Access to the time point at which the space transformation is
     * defined in this key frame.
     * @return The time point.
     */
    float getTime() const;

private:
    /**@brief The time point of this key frame.
     *
     * Store the time at which this key frame is defined.
     */
    float m_time;
    /**@brief The orientation of this key frame.
     *
     * Store the orientation part of this key frame.
     */
    glm::quat m_orientation;
    /**@brief The scale of this key frame.
     *
     * Store the scale part of this key frame.
     */
    glm::vec3 m_scale;
    /**@brief The translation of this key frame.
     *
     * Store the translation part of this key frame.
     */
    glm::vec3 m_translation;
};

#endif

#endif
