/**
 * @file HeightNode.cpp
 *
 * @see HeightNode.hpp
 */
#include <iostream>
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
    default:
	std::cerr << "Error : unknown QuadPosition type" << std::endl;
    }
}


// Interpolating between the 4 vertices of the square
float HeightNode::evalHeight(Vertex2D pos) {
 
    // Getting the required informations
    Vertex2D tlPos = topLeftData.getPosition();		
    Vertex2D brPos = bottomRightData.getPosition();
    
    Biome tlBiome = topLeftData.getBiome();
    Biome trBiome = topRightData.getBiome();
    Biome blBiome = bottomLeftData.getBiome();
    Biome brBiome = bottomRightData.getBiome();
    
    // Computing the local coordinates to interpolate
    float x    = pos.first   - tlPos.first;
    float xMax = brPos.first - tlPos.first;

    float y    = pos.second   - brPos.second;
    float yMax = tlPos.second - brPos.second;

    // Interpolation coefficients to compute
    float u, uTop, uBottom, uBlend, vLeft, vRight;

    /*
     * We do here a kind of distorted bilinear interpolation to compute
     * the height of a point given the height of the four vertices
     * of the square the point is in.
     *
     * Indeed, unlike the classical bilinear interpolation where the
     * four vertices have a symetric role, there, the four vertices are
     * charaterized by their nature (ie their biome).
     * Whereas a beach or a plain can be extended, thus affect the whole
     * square, a mountain must have a very local influence ie it must 
     * turn the surrounding plains into green mountains.
     * 
     * Thus we compute the interpolation using the following strategy
     * that consider the four biomes.
     *
     *    TL----uTop-------TR
     *    |                |
     *  vLeft            vRight
     *    |                |             ^ y
     *    |                |             |
     *    BL----uBot-------BR            |__> x
     *
     * 1) The four coefficients on the previous schema are computed
     *    ie an interpolation is done on each edge according to the following rules
     *    a) If both vertices belong to a mountain area (Mountain or Peak),
     *       we can assume that the edge belongs to a mountain are and do a
     *       sharp (linear) interpolation
     *    b) If both vertices do not belong to a mountain area, following the same
     *       idea, we do a smooth interpolation.
     *    c) Otherwise, there is a mountain and a normal land. We thus do a smooth
     *       interpolation to acknoledge the transition between the two biomes, but
     *       we control this interpolation with a scale parameter to reduce the area
     *       of influence of the mountain vertex.
     * 2) As a bilinear interpolation require only three coefficients, we blend uTop
     *    and uBot into one coefficient by simply interpolating the value u.
     * 3) Finally, with the three coefficients u, vLeft and vRight, we use the 
     *    same formula as in the bilinear interpolation.
     */   

    // Left side
    if ((tlBiome == Peak) || (tlBiome == Mountain)) {
	if((blBiome == Peak) || (blBiome == Mountain)) {
	    // Mountain region : linear interpolation
	    vLeft = linearInterpolation(y, yMax);
	} else {
	    // Border between a mountain and something else 
	    // Smooth interpolation limiting the mountain influence
	    vLeft = smooth6Interpolation(y, yMax, SCALE_MOUNTAIN);
	}
    } else {
	if((blBiome == Peak) || (blBiome == Mountain)) {
	    // Border between a mountain and something else 
	    // Smooth interpolation limiting the mountain influence
	    vLeft = smooth6Interpolation(yMax - y, yMax, SCALE_MOUNTAIN);
	} else {
	    // Smooth area
	    vLeft = smooth6Interpolation(y, yMax);
	}
    }

    // Right side
    if ((trBiome == Peak) || (trBiome == Mountain)) {
	if((brBiome == Peak) || (brBiome == Mountain)) {
	    // Mountain region : linear interpolation
	    vRight = linearInterpolation(y, yMax);
	} else {
	    // Border between a mountain and something else 
	    // Smooth interpolation limiting the mountain influence
	    vRight = smooth6Interpolation(y, yMax, SCALE_MOUNTAIN);
	}
    } else {
	if((brBiome == Peak) || (brBiome == Mountain)) {
	    // Border between a mountain and something else 
	    // Smooth interpolation limiting the mountain influence
	    vRight = smooth6Interpolation(yMax - y, yMax, SCALE_MOUNTAIN);
	} else {
	    // Smooth area
	    vRight = smooth6Interpolation(y, yMax);
	}
    }

    // Top side
    if ((tlBiome == Peak) || (tlBiome == Mountain)) {
	if((trBiome == Peak) || (trBiome == Mountain)) {
	    // Mountain region : linear interpolation
	    uTop = linearInterpolation(x, xMax);
	} else {
	    // Border between a mountain and something else 
	    // Smooth interpolation limiting the mountain influence
	    uTop = smooth6Interpolation(x, xMax, SCALE_MOUNTAIN);
	}
    } else {
	if((trBiome == Peak) || (trBiome == Mountain)) {
	    // Border between a mountain and something else 
	    // Smooth interpolation limiting the mountain influence
	    uTop = smooth6Interpolation(xMax - x, xMax, SCALE_MOUNTAIN);
	} else {
	    // Smooth area
	    uTop = smooth6Interpolation(x, xMax);
	}
    }

    // Bottom side
    if ((blBiome == Peak) || (blBiome == Mountain)) {
	if((brBiome == Peak) || (brBiome == Mountain)) {
	    // Mountain region : linear interpolation
	    uBottom = linearInterpolation(x, xMax);
	} else {
	    // Border between a mountain and something else 
	    // Smooth interpolation limiting the mountain influence
	    uBottom = smooth6Interpolation(x, xMax, SCALE_MOUNTAIN);
	}
    } else {
	if((brBiome == Peak) || (brBiome == Mountain)) {
	    // Border between a mountain and something else 
	    // Smooth interpolation limiting the mountain influence
	    uBottom = smooth6Interpolation(xMax - x, xMax, SCALE_MOUNTAIN);
	} else {
	    // Smooth area
	    uBottom = smooth6Interpolation(x, xMax);
	}
    }

    // Blend the coefficient on the x axis
    uBlend = linearInterpolation(y, yMax);
    u = uBlend*uBottom + (1-uBlend)*uTop;

    // Interpolating on the y axis
    float heightLeft  = vLeft*(bottomLeftData.getHeight())   + (1 - vLeft)*(topLeftData.getHeight());
    float heightRight = vRight*(bottomRightData.getHeight()) + (1 - vRight)*(topRightData.getHeight());
    // Interpolating on the x axis
    return (u*heightLeft + (1 - u)*heightRight);
}
