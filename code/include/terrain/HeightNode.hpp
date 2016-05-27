/**
 * @file HeightTree.hpp
 *
 * @brief Defining a node of the tree to compute the height map
 */

#ifndef HEIGHTNODE_HPP
#define HEIGHTNODE_HPP

#include "HeightData.hpp"
#include "../structures/QuadTree.hpp"

/**
 * @brief
 * The HeightNode class represents a node of the QuadTree.
 * Consequently, it encapsulates the data associated with
 * the four "blobs", one per vertex of the quad (a
 * constrained plane).
 */
class HeightNode {

public:
    /**
     * @brief Constructor
     *
     * @param tlData The top    left  blob
     * @param trData The top    right blob
     * @param blData The bottom left  blob
     * @param brData The bottom right blob
     */
    HeightNode(float newSize,
	       HeightData tlData, HeightData trData, 
	       HeightData blData, HeightData brData);

    /**
     * @brief Getter on one of the blob of the node
     *
     * @param position Position of the selected blob
     *
     * @return The selected blob
     */
    HeightData getData(QuadPosition position);
    

    /**
     * @brief Evaluate the height of a point inside the square
     *
     * @param pos The position of the point
     * 
     * @return The height of the point
     */
    float evalHeight(Vertex2D pos);


private :
    
    /// @brief The size of the square
    float size;

    /// @brief The four blobs defining the square
    HeightData topLeftData;
    HeightData topRightData;
    HeightData bottomLeftData;
    HeightData bottomRightData;

};


#endif
