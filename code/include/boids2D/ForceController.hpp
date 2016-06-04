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

  /**
   * @brief   Getter for the coefficient of separate
   * @return  Value of the coefficient of separate
   */
  float getSeparate() const;

  /**
   * @brief   Getter for the coefficient of evade
   * @return  Value of the coefficient of evade
   */
  float getEvade() const;

  /**
   * @brief   Getter for the coefficient of cohesion
   * @return  Value of the coefficient of cohesion
   */
  float getCohesion() const;

  /**
   * @brief   Getter for the coefficient of align
   * @return  Value of the coefficient of align
   */
  float getAlign() const;

  /**
   * @brief   Getter for the coefficient of stay on island
   * @return  Value of the coefficient of stay on island
   */
  float getStayOnIsland() const;

  /**
   * @brief   Getter for the coefficient of collision avoidance
   * @return  Value of the coefficient of collision avoidance
   */
  float getCollisionAvoidance() const;

  /**
   * @brief   Getter for the coefficient of follow leader
   * @return  Value of the coefficient of follow leader
   */
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