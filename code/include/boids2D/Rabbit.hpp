#ifndef RABBIT_HPP
#define RABBIT_HPP

#include "MovableBoid.hpp"

class Rabbit : public MovableBoid
{
 public:
  Rabbit(glm::vec3 location);

  Rabbit(glm::vec3 location, glm::vec3 velocity);

  Rabbit(glm::vec3 location, glm::vec3 velocity, float mass);

  Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion);

  Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, float maxSpeed, 
    float maxForce);
};

typedef std::shared_ptr<Rabbit> RabbitPtr;

#endif