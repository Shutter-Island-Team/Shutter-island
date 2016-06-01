/**
 * @file HeightNode.cpp
 *
 * @see HeightNode.hpp
 */
#include <iostream>
#include "../../include/math/InterpolationFunctions.hpp"
#include "../../include/terrain/HeightNode.hpp"
#include "../../include/terrain/MapUtils.hpp"

HeightNode::HeightNode(MapParameters& parameters,
		       float newSize,
		       HeightData tlData, HeightData trData, 
		       HeightData blData, HeightData brData) :
    m_mapParameters{parameters},
    size{newSize},
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
    	   throw std::invalid_argument("Error : unknown QuadPosition type");
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
    float u, uTop, uBottom, v, vLeft, vRight;

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
     *  vLeft    x       vRight
     *    |                |             ^ y
     *    |                |             |
     *    BL----uBot-------BR            |__> x
     *
     * 1) The four coefficients on the previous schema are computed
     *    ie an interpolation is done on each edge according to the rules
     *    explained in MapUtils   
     * 2) We use these coefficients to interpolate a height on each edge
     * 3) Then we interpolate on each axis :
     *         heightVertical(heightTop, heightBottom)
     *         heightHorizontal(heightLeft, heightRight)
     * 4) Finally we take the average height :
     *        0.5*(heightVertical + heightHorizontal)
     */   

    
    float tlHeight = topLeftData.getHeight();
    float trHeight = topRightData.getHeight();
    float blHeight = bottomLeftData.getHeight();
    float brHeight = bottomRightData.getHeight();

    // Interpolating on each edge
    // Left side
    vLeft = computeInterpolationCoefficient(m_mapParameters, tlBiome, blBiome, y, yMax);
    float heightLeft  = vLeft   * (blHeight) + (1 - vLeft)   * (tlHeight);
    // Right side
    vRight = computeInterpolationCoefficient(m_mapParameters, trBiome, brBiome, y, yMax);
    float heightRight = vRight  * (brHeight) + (1 - vRight)  * (trHeight);
    // Top side
    uTop = computeInterpolationCoefficient(m_mapParameters, tlBiome, trBiome, x, xMax);
    float heightTop   = uTop    * (tlHeight) + (1 - uTop)    * (trHeight);
    // Bottom side
    uBottom = computeInterpolationCoefficient(m_mapParameters, blBiome, brBiome, x, xMax);
    float heightBot   = uBottom * (blHeight) + (1 - uBottom) * (brHeight);

    // Interpolating on each axis
    // Vertical axis
    u = linearInterpolation(y, yMax);
    float heightVert = u * heightBot  + (1 - u) * heightTop;
    // Horizontal axis
    v = linearInterpolation(x, xMax);
    float heightHori = v * heightLeft + (1 - v) * heightRight;

    return 0.5f*(heightVert + heightHori);
}
