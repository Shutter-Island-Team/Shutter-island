/**
 * @file InterpolationFunctions.hpp
 *
 * @brief Defines a set of functions useful for interpolation
 * All of them interpolate between 0 and size, returning
 * 1 on 0 and 0 on size.
 */


#ifndef INTERPOLATIONFUNCTIONS_HPP
#define INTERPOLATIONFUNCTIONS_HPP


/**
 * @brief A linear interpolation coefficient
 *
 * @param x     The position where to interpolate (should be between 0 and size)
 * @param size  The max size to compute the coefficient
 * @param scale A scale parameter to reduce the interpolation radius
 *
 * @return The value on x interpolated between (0,1) and (size, 0)
 */

float linearInterpolation(float x, float size, float scale = 1.0f);

/**
 * @brief A smotth interpolation coefficient (polynomial of degree 6)
 *
 * @param x     The position where to interpolate (should be between 0 and size)
 * @param size  The max size to compute the coefficient
 * @param scale A scale parameter to reduce the interpolation radius
 *
 * @return The value on x interpolated between (0,1) and (size, 0)
 */
float smooth6Interpolation(float x, float size, float scale = 1.0f);

#endif
