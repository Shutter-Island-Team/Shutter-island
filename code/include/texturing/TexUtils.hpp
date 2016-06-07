#ifndef TEXUTILS_HPP
#define TEXUTILS_HPP


/**
 * @file TexUtils
 * @brief Defines some useful functions for texturing
 */


#include "./../gl_helper.hpp"
#include "./../Viewer.hpp"

#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <string>

/**
 * @brief Read textures and send them as a mip map.
 *
 * @param filename The file name of the texture
 * @param idPtr    The texture buffer identifier
 */
void sendMipMapTextures(std::vector<std::string> &filenames,
			GLuint *idPtr);


#endif
