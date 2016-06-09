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
#include "../Viewer.hpp"

class MovableBoid;
typedef std::shared_ptr<MovableBoid> MovableBoidPtr;

typedef std::shared_ptr<Matrix<MovableBoidPtr> > MatrixMovableBoidPtr;
typedef std::shared_ptr<Matrix<RootedBoidPtr> > MatrixRootedBoidPtr;

class BoidsManager
{
 public:
  /**
   * @brief Constructor for the BoidsManager
   */
  BoidsManager(MapGenerator& map, Viewer& viewer, ShaderProgramPtr& shader);

  /**
   * @brief Destructor for the BoidsManager
   */
  ~BoidsManager();
	
  /**
  * @brief Getter for the movable boids of the manager
  * @return Movable boids of the manager
  */
  const std::vector<MovableBoidPtr> & getMovableBoids() const;

  /**
   * @brief   Getter for the matrix of movable boids
   * @return  Returns the matrix of movable boids
   */
  const MatrixMovableBoidPtr & getMovableBoidsMatrix() const;

  /**
  * @brief      Getter for the rooted boids at the asked position
   * @param[in] i The index of the line of the grid
   * @param[in] j The index of the column of the grid
   * @return    Returns the list of the rooted boid at the requested position
  */
  const std::list<RootedBoidPtr> getRootedBoids(const int & i, const int & j) const;

  /**
   * @brief  Getter of the rooted boids
   * @return Return the vector of the rooted boids
   */
  const std::vector<RootedBoidPtr> getAllRootedBoids() const;

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
  MovableBoidPtr addMovableBoid(BoidType boidType, glm::vec3 location, glm::vec3 landmarkLocation, glm::vec3 velocity = glm::vec3(0,0,0));

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

  /**
   * @brief     Getter of the neighbour at a grid position
   * @param[in] i The index of the line of the grid
   * @param[in] j The index of the column of the grid
   * @return    Return the list of the movableBoid neighbour at the requested position
   */
  const std::list<MovableBoidPtr> getNeighbour(const int & i, const int & j) const;

  ForceController m_forceController; ///< Keep track of the coefficient forces

  /**
   * @brief     Get the biome giving a position
   * @param[in] x The x-coord
   * @param[in] y The y-coord
   * @return    The biome at the position
   */
  Biome getBiome(const float& x, const float& y) const;
    
  /**
   * @brief     Get the height of a position
   * @param[in] x The x-coord
   * @param[in] y The y-coord
   * @return    The value of the height at the requested position 
   */
  float getHeight(const float& x, const float& y) const;

  /**
   * @brief  Getter of the map
   * @return Return the map
   */
  MapGenerator& getMap() const;

  /**
   * @brief Remove all the dead boids from the boidManager
   */
  void removeDead();

  /**
   * @brief      Getter for the nearest lake of a boid
   * @param[in]  boid   The boid asking its nearest lake
   * @param[out] result Position of the nearest lake
   * @return     True if there it exists a lake, false otherwise
   */
  bool getNearestLake(const MovableBoidPtr & boid, glm::vec2 & result) const;

  /**
   * @brief      Getter for the nearest lake giving a position
   * @param[in]  position Position from where the request is asked
   * @param[out] result   Position of the nearest lake
   * @return     True if there it exists a lake, false otherwise
   */
  bool getNearestLake(const glm::vec2 & position, glm::vec2 & result) const;

  /**
   * @brief       Return the index of the boid giving a location
   * @param[in]   location Location asked
   * @param[out]  i line of the box
   * @param[out]  j column of the box
   */
  void coordToBox(const glm::vec3 & location, unsigned int & i, unsigned int & j) const;

  /**
   * @brief     update the boid in the grid if its poisition is different than before
   * @param[in] mvB   The considered boid
   * @param[in] iprev Previous index of the line in the grid
   * @param[in] jprev Previous index of the column in the grid
   */
  void updateBoidInGrid(MovableBoidPtr mvB, const unsigned int & iprev, const unsigned int & jprev);

  /**
   * @brief Update the tick for update of state of boids
   */
  void updateTick();

  /**
   * @brief  Check if the current tick is an update tick
   * @return True if it is an update tick, false otherwise
   */
  bool isUpdateTick() const;

  /**
   * @brief Reset the tick to 0
   */
  void resetTick();

  void addDebugMovableBoid(MovableBoidPtr m);

  void repopCarrot();

  const int & getCountCarrot() const;

  glm::vec3 computeBiomeLeaderPosition(Biome biome, float min, float max, float zPos);

  glm::vec3 computeBiomeFellowPosition(Biome biome, glm::vec3 position, float min, float max, float zPos, float radius);

  void placeBoids( Biome biomeType, int nbRabbitGroup, int nbWolfGroup, int nbForest,  int nbCarrotField);

 private:
  MapGenerator& m_map; ///< Reference of the map
  Viewer& m_viewer; ///< Reference of the viewer
  ShaderProgramPtr& m_shader; ///< Reference of the shader
  MatrixMovableBoidPtr m_movableBoids; ///< Matrix of the movable boids
  std::vector<MovableBoidPtr> m_movableBoidsVec; ///< Vector of movable boids
  MatrixRootedBoidPtr m_rootedBoids; ///< Matrix of the rooted boids
  bool isNightTime; ///< Boolean to check if it is night time

  int m_updateCoeff; ///< State of the update coefficient to check if the status of the boids need to be updated
  const int m_updatePeriod; ///< Period of tick before the update of the state of the boids

  int m_countCarrot;

  void placeRabbitGroup(Biome biomeType);

  void placeWolfGroup(Biome biomeType);

  void placeForest(Biome biomeType);

  void placeCarrotField(Biome biomeType);
};

typedef std::shared_ptr<BoidsManager> BoidsManagerPtr;

#endif