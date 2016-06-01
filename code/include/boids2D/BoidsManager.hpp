#ifndef BOIDS_MANAGER_HPP
#define BOIDS_MANAGER_HPP

#include <vector>

#include "MovableBoid.hpp"
#include "RootedBoid.hpp"
#include "BoidType.hpp"
#include "ForceController.hpp"
#include "../terrain/MapGenerator.hpp"
#include "../terrain/Biome.hpp"
#include "../structures/Matrix.hpp"

class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

class BoidsManager
{
 public:
  /**
   * @brief Constructor for the BoidsManager
   */
  BoidsManager(MapGenerator& map);

  /**
   * @briezf Destructor for the BoidsManager
   */
  ~BoidsManager();
	
  /**
  * @brief Getter for the movable boids of the manager
  * @return Movable boids of the manager
  */
  const std::vector<MovableBoidPtr>& getMovableBoids() const;

  /**
  * @brief Getter for the rooted boids of the manager
  * @return Rooted boids of the manager
  */
  const std::vector<RootedBoidPtr>& getRootedBoids() const;

  /**
  * @brief Add a rooted boid to the manager
  * @param[in] boidType Type of a boid
  * @param[in] location Location of the boid
  * @return Return the pointer of the rooted boid
  */
  RootedBoidPtr addRootedBoid(BoidType boidType, glm::vec3 location);

  /**
  * @brief Add a movable boid to the manager
  * @param[in] boidType Type of a boid
  * @param[in] location Location of the movable boid
  * @param[in] velocity Velocity of the movable boid
  * @return Return the pointer of the movable boid
  */
  MovableBoidPtr addMovableBoid(BoidType boidType, glm::vec3 location, glm::vec3 velocity = glm::vec3(0,0,0));

  /**
  * @brief Getter for the time of the day
  * @return True if it is night time, false if it is day time
  */
  bool isNight() const;

  /**
   * @brief 	Set time to be day or night. If state == true, it is daytime
   * 			else it is night time
   * @param[in] state If state is true, set timeday else night time
   */
  void setTimeDay(bool state);

  const std::vector<MovableBoidPtr> & getNeighbour(MovableBoid mvB) const;

  ForceController m_forceController; ///< Keep track of the coefficient forces

  /**
     * @brief Get the biome associated to a MovableBoid
     * 
     * @param[in] movableBoid The concerned movableBoid
     *
     * @return The biome of the position
     */
    Biome getBiome(const float& x, const float& y) const;
    
    /**
     * @brief Get the height of a MovableBoid
     *
     * @param[in] movableBoid The concerned movableBoid
     *
     * @return The biome of the position
     */
    float getHeight(const float& x, const float& y) const;

    MapGenerator& getMap() const;

    void removeDead();

  bool getNearestLake(const MovableBoidPtr & boid, glm::vec2 & result) const;

  bool getNearestLake(const glm::vec2 & position, glm::vec2 & result) const;

 private:
  MapGenerator& m_map;
  std::vector<MovableBoidPtr> m_movableBoids; ///< Vector of the movable boids
  std::vector<RootedBoidPtr> m_rootedBoids;  ///< Vector of the rooted boids
  bool isNightTime; ///< Boolean to check if it is night time
};

typedef std::shared_ptr<BoidsManager> BoidsManagerPtr;

#endif