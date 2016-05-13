# include "../include/GeometricTransformation.hpp"

GeometricTransformation::GeometricTransformation(
    const glm::vec3& translation,
    const glm::quat& orientation,
    const glm::vec3& scale )
  : m_translation{ translation },
    m_orientation{ orientation },
    m_scale{ scale }
{}

glm::mat4 GeometricTransformation::toMatrix() const
{
  glm::mat4 transformation = glm::mat4(glm::ctor::uninitialize);

  glm::mat4 rot =  glm::mat4_cast( m_orientation );
  transformation[0] = rot[0] * m_scale.x;
  transformation[1] = rot[1] * m_scale.y;
  transformation[2] = rot[2] * m_scale.z;

  transformation[3][0] = m_translation.x;
  transformation[3][1] = m_translation.y;
  transformation[3][2] = m_translation.z;
  transformation[3][3] = 1.0;
  return transformation;

}

GeometricTransformation GeometricTransformation::makeTranslation( const glm::vec3& translation )
{
  GeometricTransformation result;
  result.m_translation = translation;
  return result;
}

GeometricTransformation GeometricTransformation::makeRotation( const glm::quat& orientation )
{
  GeometricTransformation result;
  result.m_orientation = orientation;
  return result;
}

GeometricTransformation GeometricTransformation::makeScale( const glm::vec3& scale )
{
  GeometricTransformation result;
  result.m_scale = scale;
  return result;
}

void GeometricTransformation::setOrientation( const glm::quat& orientation )
{
  m_orientation = orientation;
}

const glm::quat& GeometricTransformation::getOrientation() const
{
  return m_orientation;
}

void GeometricTransformation::setTranslation( const glm::vec3& translation )
{
  m_translation = translation;
}

const glm::vec3& GeometricTransformation::getTranslation() const
{
  return m_translation;
}

void GeometricTransformation::setScale( const glm::vec3& scale )
{
  m_scale = scale;
}

const glm::vec3& GeometricTransformation::getScale() const
{
  return m_scale;
}
