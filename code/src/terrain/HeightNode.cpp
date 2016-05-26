/**
 * @file HeightNode.cpp
 *
 * @see HeightNode.hpp
 */

#include "../../include/math/InterpolationFunctions.hpp"
#include "../../include/terrain/HeightNode.hpp"

HeightNode::HeightNode(float newSize,
		       HeightData tlData, HeightData trData, 
		       HeightData blData, HeightData brData) :
    size(newSize),
    topLeftData{tlData}   , topRightData{trData},
    bottomLeftData{blData}, bottomRightData{brData}
{}



HeightData HeightNode::getData(QuadPosition position) {
    switch(position) {
    case TopLeft:
	return topLeftData;
	break;
    case TopRight:
	return topRightData;
	break;
    case BottomLeft:
	return bottomLeftData;
	break;
    case BottomRight:
	return bottomRightData;
	break;
    }
}


// Interpolating between the 4 vertices of the square
float HeightNode::evalHeight(Vertex2D pos) {
 
    Vertex2D tlPos = topLeftData.getPosition();		
    Vertex2D brPos = bottomRightData.getPosition();
    
    // Computing the local coordinates to interpolate
    float x    = pos.first   - tlPos.first;
    float xMax = brPos.first - tlPos.first;

    float y    = pos.second   - brPos.second;
    float yMax = tlPos.second - brPos.second;

    // Interpolation coefficients to compute
    float u, v;

    if ((topLeftData.getBiome() != Mountain) 
	|| (topRightData.getBiome() != Mountain)
	|| (bottomLeftData.getBiome() != Mountain)
	|| (bottomRightData.getBiome() != Mountain)) {
	// Not a mountain region : smooth interpolation
	u = smooth6Interpolation(x, xMax);
	v = smooth6Interpolation(y, yMax);
    } else {
	// Mountains : linear interpolation
	u = linearInterpolation(x, xMax);
	v = linearInterpolation(y, yMax);
    }
    
    // Interpolating on the y axis
    float heightLeft  = v*(bottomLeftData.getHeight())  + (1 - v)*(topLeftData.getHeight());
    float heightRight = v*(bottomRightData.getHeight()) + (1 - v)*(topRightData.getHeight());
    // Interpolating on the x axis
    return (u*heightLeft + (1 - u )*heightRight);
}
