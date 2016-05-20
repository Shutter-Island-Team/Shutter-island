/**
 * @file HeightTree.hpp
 *
 * @brief Defining the 'HeightMap' of the world
 */

#ifndef HEIGHTTREE_HPP
#define HEIGHTTREE_HPP

#include "HeightNode.hpp"
#include "../structures/QuadTree.hpp"


typedef QuadTree<HeightNode> HeightTree;


#endif
