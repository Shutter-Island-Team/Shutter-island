/**
 * @file QuadTree.hpp
 *
 * @brief Generic class implementing a quadtree
 */

#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <cstdlib>

enum QuadPosition {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};


template <typename T, typename S>
class QuadTree {

public:
    /**
     * @brief Constructor
     *
     * @param newContent The content of the node
     * @param newTLChild The top    left  child
     * @param newTRChild The top    right child
     * @param newBLChild The bottom left  child
     * @param newBRChild The bottom right child
     */
    QuadTree(T newContent,
	     S *newTLChild = NULL, S *newTRChild = NULL,
	     S *newBLChild = NULL, S *newBRChild = NULL);

    /**
     * @brief Getter on the content of the node
     *
     * @return The content of the node
     */
    T getContent();

    /**
     * @brief Setter on the content of the node
     * 
     * @param newContent The new content of the node
     */
    void setContent(T newContent);




    /**
     * @brief Getter on one of the child of the node
     *
     * @param position Position of the selected child
     *
     * @return The selected child
     */
    S* getChild(QuadPosition position);

    /**
     * @brief Getter on one of the child of the node
     *
     * @param position Position of the selected child
     * @param newChild Value of the selected child
     */
    void setChild(QuadPosition position, S *newChild);



private :

    /// @brief The information of the node
    T content;

    /// @brief The four children
    S *topLeftChild;
    S *topRightChild;
    S *bottomLeftChild;
    S *bottomRightChild;


};

#include "../../src/structures/QuadTree.tpp"

#endif
