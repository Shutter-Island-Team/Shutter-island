#ifndef EULER_ANGLES_HPP
#define EULER_ANGLES_HPP

#include <glm/glm.hpp>
#include <cmath>

/**
 * @brief Contains the functions required to convert  a vector
 * into angles
 */

// TODO : Euler angles


/**
 * @brief Compute the angle made by the projection
 * of the vector on the XoY plane
 * 
 * @param orientation The vector defining the orientation
 *
 * @returns The angle of the projection
 */
float toAngleXoY(glm::vec3 orientation);

/**
 * @brief Compute the angle made by the projection
 * of the vector on the YoZ plane
 * 
 * @param orientation The vector defining the orientation
 *
 * @returns The angle of the projection
 */
float toAngleYoZ(glm::vec3 orientation);

/**
 * @brief Compute the angle made by the projection
 * of the vector on the ZoX plane
 * 
 * @param orientation The vector defining the orientation
 *
 * @returns The angle of the projection
 */
float toAngleZoX(glm::vec3 orientation);






#endif
