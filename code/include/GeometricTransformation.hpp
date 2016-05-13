/**@file
 * @brief Define a geometric transformation.
 *
 * This file defines a geometric transformation, with orientation, translation
 * and scale.
 */
# ifndef GEOMETRICTRANSFORMATION_HPP_
# define GEOMETRICTRANSFORMATION_HPP_

# include <glm/glm.hpp>
# include <glm/gtc/quaternion.hpp>

/**@brief Describe a geometric transformation.
 *
 * This class is used to define a geometric transformation that will be
 * interpolated between key frames during practical 04. The orientation,
 * translation and scale components are stored separately in order to
 * be interpolated independently.
 *
 * Keep in mind that the interpolation between two orientations is on the
 * shortest great circle arc of the 4-dimensional sphere of rotations. If
 * you are not familiar with such concept:
 * - have a look on the internet
 * - avoid to have key orientations that are really different. Prefer to
 * have a sequence of rotations with smaller differences. */
class GeometricTransformation {
public:
  /**@brief Instance construction
   *
   * Construct a geometric transformation from a translation, a rotation and
   * a scale. The result is similar to a transformation matrix given by the
   * matricial product: translation * rotation * scale.
   *
   * @param orientation Orientation of the transformation, stored as a quaternion.
   * You can specify the quaternion by passing a \a glm::vec3 to it that represents
   * Euler angles.
   * @param translation Translation of the transformation.
   * @param scale Scale of the transformation. */
  GeometricTransformation(
      const glm::vec3& translation = glm::vec3{0,0,0},
      const glm::quat& orientation = glm::quat{1, 0, 0, 0},
      const glm::vec3& scale = glm::vec3{1,1,1} );

  /**@brief Convert to a transformation matrix.
   *
   * Get a transformation matrix that represent this geometric transformation. */
  glm::mat4 toMatrix() const;

  /**@brief Create a translation transformation.
   *
   * Factory to build a geometric transformation that represents only a translation.
   * @param translation The translation to represent. */
  static GeometricTransformation makeTranslation( const glm::vec3& translation );

  /**@brief Create a rotation transformation.
   *
   * Factory to build a geometric transformation that represents only a rotation.
   * @param orientation The rotation to represent.*/
  static GeometricTransformation makeRotation( const glm::quat& orientation );

  /**@brief Create a translation transformation.
   *
   * Factory to build a geometric transformation that represents only a scale.
   * @param scale The scale to represent. */
  static GeometricTransformation makeScale( const glm::vec3& scale );

  /**@brief Set the orientation component.
   *
   * Set the orientation of this transformation.
   * @param orientation The new orientation. */
  void setOrientation( const glm::quat& orientation );

  /**@brief Get the orientation component.
   *
   * Get the orientation component of this transformation.
   * @return The orientation.
   */
  const glm::quat& getOrientation() const;

  /**@brief Set the translation component.
   *
   * Set the translation of this transformation.
   * @param translation The new translation. */
  void setTranslation( const glm::vec3& translation );

  /**@brief Get the translation component.
   *
   * Get the translation component of this transformation.
   * @return The translation.
   */
  const glm::vec3& getTranslation() const;

  /**@brief Set the scale component.
   *
   * Set the scale of this transformation.
   * @param scale The new scale. */
  void setScale( const glm::vec3& scale );

  /**@brief Get the scale component.
   *
   * Get the scale component of this transformation.
   * @return The scale. */
  const glm::vec3& getScale() const;

private:
  glm::vec3 m_translation;
  glm::quat m_orientation;
  glm::vec3 m_scale;
};

# endif 
