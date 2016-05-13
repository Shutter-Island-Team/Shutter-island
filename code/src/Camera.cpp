#include "./../include/Camera.hpp"
#include <iostream>

#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

Camera::Camera()
    : m_view{ glm::lookAt( glm::vec3{0, 0, -5}, glm::vec3{}, glm::vec3{0,1,0}) },
      m_fov{ 1.04f }, m_ratio{ 1.0f }, m_znear{ 1.0f }, m_zfar{ 150.0f },
      m_mouseBehavior{ ARCBALL_BEHAVIOR }
{}

Camera::~Camera()
{

}

void Camera::animate(float time)
{
}

const glm::mat4& Camera::viewMatrix() const
{
    return m_view;
}

void Camera::setViewMatrix(const glm::mat4& view)
{
    m_view = view;
}

glm::vec3 Camera::getPosition() const
{
    return -glm::vec3( m_view[3] ) * glm::mat3( m_view );
}

glm::vec3 Camera::getRight() const
{
    return glm::vec3( m_view[0][0], m_view[1][0], m_view[2][0] );
}

glm::vec3 Camera::getUp() const
{
    return glm::vec3( m_view[0][1], m_view[1][1], m_view[2][1] );
}

glm::vec3 Camera::getForward() const
{
    return glm::vec3( -m_view[0][2], -m_view[1][2], -m_view[2][2] );
}

void Camera::setPosition( const glm::vec3& pos )
{
    for( int i = 0; i < 3; ++ i )
    {
        m_view[3][i] = float(0);
        for( int j = 0; j < 3; ++ j )
        {
            m_view[3][i] -= m_view[j][i] * pos[j];
        }
    }
}

void Camera::setRight( const glm::vec3& right )
{
    glm::vec3 minus_pos = glm::vec3( m_view[3] ) * glm::mat3( m_view );
    m_view[0][0] = right.x;
    m_view[1][0] = right.y;
    m_view[2][0] = right.z;
    m_view[3][0] = dot( right, minus_pos );
}

void Camera::setUp( const glm::vec3& up )
{
  glm::vec3 minus_pos = glm::vec3( m_view[3] ) * glm::mat3( m_view );
  m_view[0][1] = up.x;
  m_view[1][1] = up.y;
  m_view[2][1] = up.z;
  m_view[3][1] = dot( up, minus_pos );
}

void Camera::setForward( const glm::vec3& forward )
{
  glm::vec3 pos = -glm::vec3( m_view[3] ) * glm::mat3( m_view );
  m_view[0][0] = -forward.x;
  m_view[1][0] = -forward.y;
  m_view[2][0] = -forward.z;
  m_view[3][0] = dot( forward, pos );
}

const glm::mat4& Camera::projectionMatrix() const
{
    return m_projection;
}

void Camera::setProjectionMatrix(const glm::mat4& projection)
{
    m_projection = projection;
}

float Camera::fov() const
{
    return m_fov;
}

float Camera::ratio() const
{
    return m_ratio;
}

float Camera::znear() const
{
    return m_znear;
}

float Camera::zfar() const
{
    return m_zfar;
}

void Camera::setFov( const float& v )
{
    m_fov = v;
    m_projection = glm::perspective( m_fov, m_ratio, m_znear, m_zfar );
}

void Camera::setRatio( const float& v )
{
    m_ratio = v;
    m_projection = glm::perspective( m_fov, m_ratio, m_znear, m_zfar );
}

void Camera::setZfar( const float& v )
{
    m_zfar = v;
    m_projection = glm::perspective( m_fov, m_ratio, m_znear, m_zfar );
}

void Camera::setZnear( const float& v )
{
    m_znear = v;
    m_projection = glm::perspective( m_fov, m_ratio, m_znear, m_zfar );
}

Camera::CAMERA_MOUSE_BEHAVIOR Camera::getMouseBehavior() const
{
    return m_mouseBehavior;
}

void Camera::setMouseBehavior( const CAMERA_MOUSE_BEHAVIOR& v )
{
    m_mouseBehavior = v;
    if( m_mouseBehavior == ARCBALL_BEHAVIOR )
      {
        glm::vec3 pos = getPosition();
        glm::vec3 up = getUp();
        m_view = glm::lookAt(
            getPosition(),
            glm::vec3{},
            getUp());
      }
}

void Camera::update( float dx, float dy )
{
    switch( m_mouseBehavior )
    {
    case ARCBALL_BEHAVIOR:
    {
        glm::mat4 rotation = glm::rotate( glm::mat4( glm::mat3(m_view) ), dx, getUp() );
        rotation = glm::rotate( rotation, dy, getRight() );

        m_view[0] = rotation[0];
        m_view[1] = rotation[1];
        m_view[2] = rotation[2];

        setPosition( -glm::vec3(m_view[3]) * glm::mat3(rotation) );
    }
        break;

    case SPACESHIP_BEHAVIOR:
    {
        const float cx = float( std::cos( dx ) );
        const float sx = float( std::sin( dx ) );
        const float cy = float( std::cos( dy ) );
        const float sy = float( std::sin( dy ) );

        glm::mat3 rotation = glm::mat3( m_view );
        glm::vec3 minus_pos = glm::vec3( m_view[3] ) * rotation;
        // build a rotation matrix to apply to the current rotation:
        //
        rotation = glm::mat3(
                    glm::vec3( cx, sx*sy,-sx*cy),
                    glm::vec3(  0,    cy,    sy),
                    glm::vec3( sx,-cx*sy, cx*cy) ) *rotation;
        for( int col = 0; col < 3; ++ col )
            for( int lin = 0; lin < 3; ++ lin )
            {
                m_view[col][lin] = rotation[col][lin];
            }
        m_view[3][0] = dot( minus_pos, getRight() );
        m_view[3][1] = dot( minus_pos, getUp() );
        m_view[3][2] = dot( minus_pos, -getForward() );
    }
        break;

    default:
        assert(false);
    }
}
