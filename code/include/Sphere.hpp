#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <glm/glm.hpp>

class Sphere
{
    public:
        ~Sphere();
        Sphere(const glm::vec3& c, const float& r);

        void setRadius(const float& r);
        const float& radius() const;

        void setCenter(const glm::vec3& c);
        const glm::vec3& center() const;

    private:
        glm::vec3 m_c;
        float m_r;
};

#endif //SPHERE_HPP
