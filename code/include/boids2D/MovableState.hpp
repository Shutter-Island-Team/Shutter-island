#ifndef MOVABLE_STATE_HPP
#define MOVABLE_STATE_HPP

#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include "MovableBoid.hpp"

class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class MovableState 
{
 public:
  /**
   * @brief Return the acceleration and reset the acceleration of the boid
   */
  glm::vec3 computeAcceleration(MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;

 private:
  virtual glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const = 0;

 protected:
  glm::vec3 seek(const MovableBoid& b, const glm::vec3 & target) const;
  glm::vec3 flee(const MovableBoid& b, const glm::vec3 & repulsionPoint) const;
  glm::vec3 wander(const MovableBoid& b) const;
  glm::vec3 arrive(const MovableBoid& b, const glm::vec3 & target) const;
  glm::vec3 stayWithinWalls(const MovableBoid& b) const;
  glm::vec3 separate(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB) const;
  glm::vec3 align (const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB) const;
  glm::vec3 cohesion (const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB) const;
  glm::vec3 pursuit (const MovableBoid & hunter, const MovableBoid & target, const float & dt) const;
  glm::vec3 evade(const MovableBoid & prey, const MovableBoid & hunter, const float & dt) const;


  /**
   * @brief Forecast of the position of a boid
   * @param[in] b The boid to forecast the position
   * @param[in] dt Step of time
   * @param[in] cst Constante for the hunt
   */
  glm::vec3 positionForecast(const MovableBoid & b, const float & dt, const float & cst) const;

};

class TestState : public MovableState 
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class WalkState : public MovableState 
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class StayState : public MovableState 
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class SleepState : public MovableState 
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class FleeState : public MovableState 
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class FindFoodState : public MovableState 
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class EatState : public MovableState 
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class FindWaterState : public MovableState 
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class DrinkState : public MovableState 
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class MateState : public MovableState 
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class AttackState : public MovableState
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};

class LostState : public MovableState
{
 private:
  glm::vec3 computeNewForces(const MovableBoid& b, const std::vector<MovableBoidPtr> & mvB, const float & dt) const;
};


// Return the closest animal of b of the good type in the vector.
// Need to be seen too.
MovableBoidPtr closestAnimal(const MovableBoid & b, const BoidType & type, const std::vector<MovableBoidPtr> & mvB);

#endif