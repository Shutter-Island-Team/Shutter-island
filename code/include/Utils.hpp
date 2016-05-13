#ifndef UTILS_HPP
#define UTILS_HPP

/**@file
 * @brief Some useful functions to ease the development.
 */

#include "GeometricTransformation.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"

#include <array>
#include <vector>
#include <utility>
#include <glm/glm.hpp>

/** @brief Get a random number uniformly sampled in [a,b[.
 *
 * This function returns a random number in [a,b[.
 * @return The random number generated. */
float random(float a, float b);

/** @brief Get a random valid color.
 *
 * This function creates for you a random color you can use to render your vertices.
 * @return The random color created. */
glm::vec4 randomColor();

/** @brief Get a color to represent a one dimensional parameter.
 *
 * This function allows to visually represent one dimensional parameter
 * thanks to a color code: cold colors (blue) for small values and hot colors
 * (red) for big values.
 *
 * @param factor The factor to visually represent.
 * @param low The minimum expected value for the factor (will return a blue color).
 * @param high The maximum expected value for the factor (will return a red color).
 * @return The generated color vector. */
glm::vec4 getColor( const float& factor, const float& low, const float& high );

void getUnitIndexedCube(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::ivec3>& indices);
void getUnitPlane(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2> &texCoords);
void getUnitCube(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2> &texCoords);
void getUnitCylinder(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const unsigned int& slices);
void getUnitSphere(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const unsigned int& strips, const unsigned int& slices);
void getTorus(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const float& a, const float& c, const unsigned int& strips, const unsigned int& slices);
void getUnitCone(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const unsigned int &strips, const unsigned int& slices);

#endif //UTILS
