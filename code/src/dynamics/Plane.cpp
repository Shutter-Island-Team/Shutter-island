#include "./../../include/dynamics/Plane.hpp"
#include <glm/gtc/matrix_access.hpp>


Plane::Plane(const glm::vec3& normal,
	     const glm::vec3& point )
    : m_d{ dot( glm::normalize(normal), point ) },
    m_origin{glm::vec3(point)}
{
    this->setNormal(glm::normalize(normal));
}

Plane::Plane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
    m_origin = glm::vec3(a);
    this->setNormal(glm::normalize( glm::cross(b-a, c-a)));
    m_d = glm::dot(m_n, a);
}

glm::vec3 Plane::projectOnPlane(const glm::vec3& p)
{
    glm::vec3 planePoint = m_d*m_n;
    glm::vec3 v = p-planePoint;
    float dist = glm::dot(v,m_n);
    glm::vec3 projection = p - dist*m_n;
    return projection;
}

void Plane::setDistanceToOrigin(const float& d)
{
    m_d = d;
}

const float& Plane::distanceToOrigin() const
{
    return m_d;
}

void Plane::setNormal(const glm::vec3& n)
{
    m_n = n;
    
    this->computeLocalFrame();
}

const glm::vec3& Plane::normal() const
{
    return m_n;
}


void Plane::boundX(bool limit, float min, float max) {
    m_isLimitedX = limit;
    m_xMin = min;
    m_xMax = max;
}


void Plane::boundY(bool limit, float min, float max) {
    m_isLimitedY = limit;
    m_yMin = min;
    m_yMax = max;
}

bool Plane::projectionBelongsToPlane(const glm::vec3 &p) {
    if ((! m_isLimitedX) && (! m_isLimitedY))
	return true;
    
    glm::vec3 projection = this->projectOnPlane(p);
    glm::vec3 localVector = projection - m_origin;

    float x = glm::dot(localVector, m_xAxis);
    float y = glm::dot(localVector, m_yAxis);
    return ((((! m_isLimitedX)) || ((m_xMin <= x) && (x <= m_xMax)))
	    && (((! m_isLimitedY)) || ((m_yMin <= y) && (y <= m_yMax))));
	
}


void Plane::computeLocalFrame() {
    m_xAxis = glm::vec3(-m_n[1], m_n[0], 0.0);
    m_yAxis = glm::vec3(0.0, 0.0, 1.0);	
}

