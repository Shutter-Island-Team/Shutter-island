/**
 * @file HeightNode.cpp
 *
 * @see HeightNode.hpp
 */

#include "../../include/terrain/HeightNode.hpp"

HeightNode::HeightNode(HeightBlob tlBlob, HeightBlob trBlob, 
		       HeightBlob blBlob, HeightBlob brBlob) :
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
