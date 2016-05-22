/**
 * @file BlobFunctions.hpp
 *
 * @brief Defines some blobs functions
 */


#ifndef BLOBFUNCTIONS_HPP
#define BLOBFUNCTIONS_HPP


/**
 * @brief A sharp blob (linear)
 *
 * @param x     The position where the blob is evaluated (should be positive and < size)
 * @param size  The size of the area affected by the blob
 * @param scale The size of the blob regarding the area
 */

float blobSharp(float x, float size, float scale = 1.0f);

/**
 * @brief A very smooth blob (polynomial of degree 6)
 *
 * @param x     The position where the blob is evaluated (should be positive and < size)
 * @param size  The size of the area affected by the blob
 * @param scale The size of the blob regarding the area
 */

float blobSmooth6(float x, float size, float scale = 1.0f);

#endif
