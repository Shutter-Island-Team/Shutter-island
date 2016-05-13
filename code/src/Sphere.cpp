#include "./../include/Sphere.hpp"

Sphere::~Sphere(){}

Sphere::Sphere(const glm::vec3& c, const float& r)
{
    m_c = c;
    m_r = r;
}

void Sphere::setRadius(const float& r)
{
    m_r = r;
}

const float& Sphere::radius() const
{
    return m_r;
}

void Sphere::setCenter(const glm::vec3& c)
{
    m_c = c;
}

const glm::vec3& Sphere::center() const
{
    return m_c;
}
