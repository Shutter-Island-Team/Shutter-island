#ifndef RABBIT_HPP
#define RABBIT_HPP

#include "MovableBoid.hpp"

/**
 * @class Rabbit
 * @brief Representation of a rabbit
 */
class Rabbit : public MovableBoid
{
 public:
  /**
   * @brief 	Constructor for Rabbit
   * @param[in]	location	Initial position of the rabbit
   * @param[in]	parameters 	Parameter of the rabbit
   */
  Rabbit(glm::vec3 location, MovableParametersPtr parameters);

  /**
   * @brief 	Constructor for Rabbit
   * @param[in]	location	Initial position of the rabbit
   * @param[in]	velocity	Initial velocity of the rabbit
   * @param[in]	parameters 	Parameter of the rabbit
   */
  Rabbit(glm::vec3 location, glm::vec3 velocity, MovableParametersPtr parameters);

  /**
   * @brief 	Constructor for Rabbit
   * @param[in]	location	Initial position of the rabbit
   * @param[in]	velocity	Initial velocity of the rabbit
   * @param[in]	mass		Mass of the rabbit
   * @param[in]	parameters 	Parameter of the rabbit
   */
  Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
		MovableParametersPtr parameters);


 protected:
  /**
   * @brief     Constructor for Rabbit
   * @param[in] location    Initial position of the rabbit
   * @param[in] velocity    Initial velocity of the rabbit
   * @param[in] mass        Mass of the rabbit
   * @param[in] parameters  Parameter of the rabbit
   * @param[in] amountFood  Amount of food of the boid
   */
  Rabbit(glm::vec3 location, glm::vec3 velocity, float mass,
    MovableParametersPtr parameters, float amountFood);
};

typedef std::shared_ptr<Rabbit> RabbitPtr;

#endif