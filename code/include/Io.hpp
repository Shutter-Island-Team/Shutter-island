#ifndef IO_HPP
#define IO_HPP

/**@file
 *@brief Input/Output functions.
 *
 * Currently, this file only contains I/O functions for OBJ meshes.*/

#include <vector>
#include <glm/glm.hpp>
#include <string>

/**@brief Collect mesh data from an OBJ file.
 *
 * This function opens an OBJ mesh file to collect information such
 * as vertex position, vertex indices of a face, vertex normals and vertex
 * texture coordinates.
 *
 * @param filename The path to the mesh file.
 * @param positions The vertex positions.
 * @param indices The vertex indices of faces.
 * @param normals The vertex normals.
 * @param texcoords The vertex texture coordinates.
 * @return False if import failed, true otherwise.
 */
bool read_obj(
        const std::string& filename, 
        std::vector<glm::vec3>& positions, 
        std::vector<unsigned int>& indices,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texcoords
        );

#endif //IO_HPP
