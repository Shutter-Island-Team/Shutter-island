#ifndef WOLF_HPP
#define WOLF_HPP

#include "MovableBoid.hpp"

class Wolf : public MovableBoid
{
 public:
  Wolf(glm::vec3 location);

  Wolf(glm::vec3 location, glm::vec3 velocity);

  Wolf(glm::vec3 location, glm::vec3 velocity, float mass);

  Wolf(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion);

  Wolf(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, float maxSpeed, 
    float maxForce);
};

typedef std::shared_ptr<Wolf> WolfPtr;

#endif