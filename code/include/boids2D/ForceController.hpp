#ifndef FORCE_CONTROLLER
#define FORCE_CONTROLLER

/**
 * @class ForceController
 * @brief Save the value of coefficient of forces
 */
class ForceController
{
 public:
  /**
   * @brief Constructor of ForceController which read from file
   */
  ForceController();

  float getSeparate() const;
  float getEvade() const;
  float getCohesion() const;
  float getAlign() const;
  float getStayOnIsland() const;
  float getCollisionAvoidance() const;
  float getFollowLeader() const;

 private:
  float m_separate; ///< Value of the separation coefficient
  float m_evade; ///< Value of the evade coefficient
  float m_cohesion; ///< Value of the cohesion coefficient
  float m_align; ///< Value of the align coefficient
  float m_stayOnIsland; ///< Value of the stayOnIsland coefficient
  float m_collisionAvoidance; ///< Value of the collisionAvoidance coefficient
  float m_followLeader; ///< Value of the follow leader coefficient  
};

#endif