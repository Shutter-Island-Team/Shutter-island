/**
 * @file HeightTree.hpp
 *
 * @brief Defining a node of the tree to compute the height map
 */

#ifndef HEIGHTNODE_HPP
#define HEIGHTNODE_HPP

#include "HeightBlob.hpp"
#include "../structures/QuadTree.hpp"

class HeightNode {

    /**
     * @brief Constructor
     *
     * @param tlBlob The top    left  blob
     * @param trBlob The top    right blob
     * @param blBlob The bottom left  blob
     * @param brBlob The bottom right blob
     */
    HeightNode(HeightBlob tlBlob, HeightBlob trBlob, 
	       HeightBlob blBlob, HeightBlob brBlob);

    /**
     * @brief Getter on one of the blob of the node
     *
     * @param position Position of the selected blob
     *
     * @return The selected blob
     */
    HeightBlob getBlob(QuadPosition position);
    


private :
    
    /// @brief The four blobs defining the square
    HeightBlob topLeftBlob;
    HeightBlob topRightBlob;
    HeightBlob bottomLeftBlob;
    HeightBlob bottomRightBlob;

};


#endif
