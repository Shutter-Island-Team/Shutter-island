/**
 * @file SeaRenderable.ccp
 *
 * @see SeaRenderable.hpp
 */ 

#include "./../../include/terrain/SeaRenderable.hpp"

#include "./../../include/terrain/MapRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"
#include "./../../include/terrain/MapUtils.hpp"
#include "./../../include/texturing/TexUtils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

SeaRenderable::SeaRenderable(ShaderProgramPtr shaderProgram, 
			     const glm::vec3& p1,
			     const glm::vec3& p2,
			     const glm::vec3& p3,
			     const glm::vec3& p4) :
    HierarchicalRenderable(shaderProgram)
{
    // The geometry
    m_positions.push_back(p1);
    m_positions.push_back(p2);
    m_positions.push_back(p3);

    m_positions.push_back(p1);
    m_positions.push_back(p3);
    m_positions.push_back(p4);

    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));

    // Create texture
    std::vector<std::string> filenames;
    std::string name= "../textures/shutter_texture_water";
    std::string extension = ".png";
    int minRes = 2;
    int maxRes = 8; 
    for (int i = minRes; i <= maxRes; ++i) {
	filenames.push_back(name + std::to_string(i) + extension);
    }
    sendMipMapTextures(filenames, &m_textureId);
}


SeaRenderable::~SeaRenderable()
{
    /*
     * Disabling the GPU buffers used while rendering.
     */
    glcheck(glDeleteBuffers(1, &m_pBuffer));
}


void SeaRenderable::do_draw()
{
    /*
     * Drawing geometric data
     */

    // Position
    int positionLocation        = m_shaderProgram->getAttributeLocation("coord");

     // Model matrix
    int modelLocation           = m_shaderProgram->getUniformLocation("modelMat");

    // The texture scale parameter
    int scaleTextureLocation    = m_shaderProgram->getUniformLocation("scaleTexture");

    // Texture
    int seaTextureLocation      = m_shaderProgram->getUniformLocation("seaTex"); 


    //Send material uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);


    // Positions
    if(positionLocation != ShaderProgram::null_location)
	{
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        glcheck(glVertexAttribPointer(
                    positionLocation, 
                    3,
                    GL_FLOAT, 
                    GL_FALSE, 
                    0, 
                    (void*)0
                )
        );
	}    


    // Model matrix
    if(modelLocation != ShaderProgram::null_location)
	{
	    glcheck(glUniformMatrix4fv(
				       modelLocation, 
				       1, 
				       GL_FALSE, 
				       glm::value_ptr(getModelMatrix())
				       )
		    );
	}

    // Texture scale
    if (scaleTextureLocation != ShaderProgram::null_location)
    {
        glcheck(glUniform1f(scaleTextureLocation, 
			    20.0f));
    }

    // Sea Texture
    if (seaTextureLocation != ShaderProgram::null_location)
	{
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_textureId));
        glcheck(glUniform1i(seaTextureLocation, 0)); 
	}
    
    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES, 0, m_positions.size()));
    
    /*
     * Disabling the buffers.
     */
    if(positionLocation != ShaderProgram::null_location)
	{
	    glcheck(glDisableVertexAttribArray(positionLocation));
	}

}


void SeaRenderable::do_animate( float time )
{}

void SeaRenderable::setMaterial(const MaterialPtr& material) {
    m_material = material;
}
