/**
 * @file HeightNode.cpp
 *
 * @see HeightNode.hpp
 */

#include "../../include/terrain/HeightNode.hpp"

HeightNode::HeightNode(float newSize,
		       HeightBlob tlBlob, HeightBlob trBlob, 
		       HeightBlob blBlob, HeightBlob brBlob) :
    size(newSize),
    topLeftBlob{tlBlob}   , topRightBlob{trBlob},
    bottomLeftBlob{blBlob}, bottomRightBlob{brBlob}
{}



HeightBlob HeightNode::getBlob(QuadPosition position) {
    switch(position) {
    case TopLeft:
	return topLeftBlob;
	break;
    case TopRight:
	return topRightBlob;
	break;
    case BottomLeft:
	return bottomLeftBlob;
	break;
    case BottomRight:
	return bottomRightBlob;
	break;
    }
}


float HeightNode::evalHeight(Vertex2D pos) {
    return (topLeftBlob.evalHeight(pos, size)    + topRightBlob.evalHeight(pos, size) +
	    bottomLeftBlob.evalHeight(pos, size) + bottomRightBlob.evalHeight(pos, size));
}
