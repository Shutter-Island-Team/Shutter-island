#ifndef BOID_HPP
#define BOID_HPP

#include <glm/glm.hpp>
#include <memory>

#include <vector>
#include "BoidType.hpp"

/**
 * @class Boid
 * @brief Parent class for a Boid (can be rooted or movable)
 */
class Boid
{
 public:
  /**
   * @brief		Constructor for a Boid
   * @param[in]	location 	The initial position
   * @param[in]	t 			Type of the boid
   */
  Boid(glm::vec3 location, BoidType t);

  /**
   * @brief Getter for the location
   */
  glm::vec3 getLocation() const;

  /**
   * @brief 	Setter for the location
   * @param[in]	location The new location
   */
  void setLocation(const glm::vec3 & location);

  /**
   * @brief Getter of the angle
   */
  float getAngle() const;

  /**
   * @brief 	Setter of the angle
   * @param[in]	angle The new angle
   */
  void setAngle(const float & angle);

  /**
   * @brief   Getter of the type of the boid
   * @return  Type of the boid
   */
  BoidType getBoidType() const;

  /**
   * @brief Getter of the size of the boid
   */
  float getScale() const;

  /**
   * @brief Setter of the size of the boid
   */
  void setScale(const float & scale);

  void disapear();

  bool toDisplay() const;

  bool isFoodRemaining() const;

  void decreaseFoodRemaining();

 protected:
  /**
   * @brief   Constructor for a Boid
   * @param[in] location  The initial position
   * @param[in] t       Type of the boid
   */
  Boid(glm::vec3 location, BoidType t, int amountFood);
  
  glm::vec3 m_location; ///< Position of the boid

 private:
 	BoidType m_boidType; ///< Type of the boid @see BoidType.hpp
  float m_angle; ///< Angle of position of the boid
  float m_scale; ///< Size of the boid
  bool m_display;
  int m_amountFood; ///< @todo : change this in int
};

typedef std::shared_ptr<Boid> BoidPtr;

#endif
