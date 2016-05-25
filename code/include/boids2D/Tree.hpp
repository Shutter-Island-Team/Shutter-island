#ifndef TREE_BOID_HPP
#define TREE_BOID_HPP

#include "RootedBoid.hpp"
#include "BoidType.hpp"

/**
 * @class Tree
 * @brief Representation of a tree
 */
class Tree : public RootedBoid
{
 public:
  /**
   * @brief 	Constructor for Tree
   * @param[in] location Initial location
   */
  Tree(glm::vec3 location);
};

typedef std::shared_ptr<Tree> TreePtr;

#endif