#ifndef BOID_HPP
#define BOID_HPP

#include <glm/glm.hpp>
#include <memory>

#include <vector>
#include "BoidType.hpp"

class Boid
{
 public:
  /**
   * @brief
   * @param[in]
   * @param[in]
   */
  Boid(glm::vec3 location, BoidType t);

  glm::vec3 getLocation() const;

  void setLocation(const glm::vec3 & location);

  float getAngle() const;

  void setAngle(const float & angle);

  float getMinDistance() const;

  void setMinDistance(const float & minDistance);

  BoidType getBoidType() const;

 protected:
 	glm::vec3 m_location;

 private:
 	float m_angle;
 	BoidType m_boidType;

 	float m_minDistance = 1;
};

typedef std::shared_ptr<Boid> BoidPtr;

#endif
