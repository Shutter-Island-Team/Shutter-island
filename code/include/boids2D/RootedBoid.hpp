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

  float getRadius() const;

 private:
  float m_radius;
};


typedef std::shared_ptr<RootedBoid> RootedBoidPtr;

#endif