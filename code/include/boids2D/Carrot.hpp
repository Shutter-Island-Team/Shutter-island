#ifndef CARROT_BOID_HPP
#define CARROT_BOID_HPP

#include "RootedBoid.hpp"
#include "BoidType.hpp"

/**
 * @class Carrot
 * @brief This class is a rooted boid to represent a carrot
 */
class Carrot : public RootedBoid
{
 public:
  /**
   * @brief 	Constructor of a class carrot
   * @param[in] location Position of the object
   */
  Carrot(glm::vec3 location);
};

typedef std::shared_ptr<Carrot> CarrotPtr;

#endif