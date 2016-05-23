#ifndef RABBIT_HPP
#define RABBIT_HPP

#include "MovableBoid.hpp"

class Rabbit : public MovableBoid
{
 public:
  Rabbit(glm::vec3 location, MovableParameters* parameters);

  Rabbit(glm::vec3 location, glm::vec3 velocity, MovableParameters* parameters);

  Rabbit(glm::vec3 location, glm::vec3 velocity, float mass, MovableParameters* parameters);

  Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, MovableParameters* parameters);

  Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
    float angleView, float distViewSeparate, float distViewCohesion, float maxSpeed, 
    float maxForce, MovableParameters* parameters);
};

typedef std::shared_ptr<Rabbit> RabbitPtr;

#endif