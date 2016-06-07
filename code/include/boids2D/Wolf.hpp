#ifndef WOLF_HPP
#define WOLF_HPP

#include "MovableBoid.hpp"

/**
 * @class Wolf
 * @brief Representation of a wolf
 */
class Wolf : public MovableBoid
{
 public:
  /**
   * @brief 	Constructor for Wolf
   * @param[in]	location	Initial position of the wolf
   * @param[in]	parameters 	Parameter of the wolf
   */
  Wolf(glm::vec3 location, glm::vec3 landmarkPosition, MovableParametersPtr parameters);

  /**
   * @brief 	Constructor for Wolf
   * @param[in]	location	Initial position of the wolf
   * @param[in]	velocity	Initial velocity of the wolf
   * @param[in]	parameters 	Parameter of the wolf
   */
  Wolf(glm::vec3 location, glm::vec3 landmarkPosition, glm::vec3 velocity, MovableParametersPtr parameters);

  /**
   * @brief 	Constructor for Wolf
   * @param[in]	location	Initial position of the wolf
   * @param[in]	velocity	Initial velocity of the wolf
   * @param[in]	mass		Mass of the wolf
   * @param[in]	parameters 	Parameter of the wolf
   */
  Wolf(glm::vec3 location, glm::vec3 landmarkPosition, glm::vec3 velocity, float mass,
		MovableParametersPtr parameters);

 private:
  BoidType getType() const;
};

typedef std::shared_ptr<Wolf> WolfPtr;

#endif