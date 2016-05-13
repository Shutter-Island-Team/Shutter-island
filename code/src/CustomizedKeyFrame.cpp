#include "./../include/CustomizedKeyFrame.hpp"
#include <iostream>

#if 0
CustomizedKeyFrame::CustomizedKeyFrame(float time, const glm::quat& orientation, const glm::vec3& scale, const glm::vec3& translation ) :
    m_time{ time },
    m_orientation{ orientation },
    m_scale{ scale },
    m_translation{ translation }
{}

CustomizedKeyFrame::CustomizedKeyFrame(float time, const glm::mat4& transformation ) :
    m_time{ time },
    m_scale{ glm::length(transformation[0]), glm::length(transformation[1]), glm::length(transformation[2]) },
    m_translation{ glm::vec3( transformation[3] ) }
{
    glm::mat3 temp = glm::mat3( transformation );
    temp[0] *= float( 1.0 / m_scale.x );
    temp[1] *= float( 1.0 / m_scale.y );
    temp[2] *= float( 1.0 / m_scale.z );
    m_orientation = glm::quat_cast( temp );
}

CustomizedKeyFrame::CustomizedKeyFrame( const CustomizedKeyFrame& keyFrame)
{
    m_time = keyFrame.m_time;
    m_orientation = keyFrame.m_orientation;
    m_scale = keyFrame.m_scale;
    m_translation = keyFrame.m_translation;
}

static glm::mat4 from_scale_rotation_translation(const glm::vec3& scale, const glm::quat& orientation, const glm::vec3& translation )
{
    glm::mat4 res = glm::mat4(1.0);
    glm::mat4 rot =  glm::mat4_cast( orientation );

    res[0] = rot[0] * scale.x;
    res[1] = rot[1] * scale.y;
    res[2] = rot[2] * scale.z;

    res[3][0] = translation.x;
    res[3][1] = translation.y;
    res[3][2] = translation.z;

    return res;
}

glm::mat4 CustomizedKeyFrame::interpolate(const CustomizedKeyFrame& k1, const CustomizedKeyFrame& k2, float time )
{
    if( time >= k2.m_time )
        return from_scale_rotation_translation( k2.m_scale, k2.m_orientation, k2.m_translation );
    else if( time <= k1.m_time )
        return from_scale_rotation_translation( k1.m_scale, k1.m_orientation, k1.m_translation );

    float inv_dt = float( 1.0 ) / ( k2.m_time - k1.m_time );
    float t1 = (k2.m_time - time) * inv_dt;
    float t2 = (time - k1.m_time) * inv_dt;

    glm::vec3 scale = k1.m_scale * t1 + k2.m_scale * t2;
    glm::quat orientation = glm::normalize( k1.m_orientation * t1 + k2.m_orientation * t2 );
    glm::vec3 translation = k1.m_translation * t1 + k2.m_translation * t2;

    glm::mat4 result = from_scale_rotation_translation( scale, orientation, translation);

    return result;
}

float CustomizedKeyFrame::getTime() const
{
  return m_time;
}

#endif
