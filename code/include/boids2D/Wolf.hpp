#ifndef WOLF_HPP
#define WOLF_HPP

#include "MovableBoid.hpp"

class Wolf : public MovableBoid
{
 public:
  Wolf(glm::vec3 location, MovableParameters* parameters);

  Wolf(glm::vec3 location, glm::vec3 velocity, MovableParameters* parameters);

  Wolf(glm::vec3 location, glm::vec3 velocity, float mass,
		MovableParameters* parameters);
};

typedef std::shared_ptr<Wolf> WolfPtr;

#endif