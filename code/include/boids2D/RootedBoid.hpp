#ifndef ROOTED_BOID_HPP
#define ROOTED_BOID_HPP

#include "Boid.hpp"

/**
 * @class RootedBoid
 * @brief This class respresent a boid which cannot move
 */
class RootedBoid : public Boid
{
 public:
  /**
   * @brief Constructor for a RootedBoid
   * @param[in] location Location of the object
   * @param[in] t Type of the RootedBoid
   */
  RootedBoid(glm::vec3 location, BoidType t);

  /**
   * @brief Getter for the radius of the boid
   * @return Return the radius
   */
  float getRadius() const;
  
  /**
   * @brief Constructor for a RootedBoid
   * @param[in] location    Location of the object
   * @param[in] t           Type of the RootedBoid
   * @param[in] amountFood  Amount of food of the boid
   */
  RootedBoid(glm::vec3 location, BoidType t, int amountFood);

  RootedBoid(glm::vec3 location, BoidType t, float radius);

  RootedBoid(glm::vec3 location, BoidType t, float radius, int amountFood);

 private:
  float m_radius; ///< Radius of the boid to have "place" to live
};


typedef std::shared_ptr<RootedBoid> RootedBoidPtr;

#endif