/**
 * @file LakeRenderable.ccp
 *
 * @see LakeRenderable.hpp
 */ 

#include "./../../include/terrain/LakeRenderable.hpp"

#include "./../../include/terrain/MapRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"
#include "./../../include/terrain/MapUtils.hpp"
#include "./../../include/texturing/TexUtils.hpp"

#include <cfloat>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

#include <iostream>

LakeRenderable::LakeRenderable(
    ShaderProgramPtr shaderProgram,
    std::vector<
        std::pair<
            std::vector<int>, 
            std::vector<glm::vec3> 
        >
    >& lakesTriangles
)
    :   HierarchicalRenderable(shaderProgram),
        m_positions(0)
{
    /*
     * Pushing the triangles into the positions buffer.
     */
    for (
        auto globalIt = lakesTriangles.begin();
        globalIt != lakesTriangles.end();
        globalIt++
    )
    {
        /*
         * So as to set the altitude of the lake coherently, we compute the
         * average altitude of the vertices constituting the connexe lakes.
         */
        float averageAltitude = 0.0;
        for (
            auto localIt = globalIt->second.begin();
            localIt != globalIt->second.end();
            localIt++
        )
        {
            averageAltitude += localIt->z;
        }
        averageAltitude /= (float)(globalIt->second.size());
        
        /*
         * If the average altitude is below the sea altitude, we slightly
         * increase it.
         */
        //if (averageAltitude < 0.0) {
        //  averageAltitude = 0.3;
        //}

        for (
            auto localIt = globalIt->second.begin();
            localIt != globalIt->second.end();
            localIt++
        )
        {
            m_positions.push_back(glm::vec3(localIt->x, localIt->y, averageAltitude));
        }
    }

    /*
     * Creating the position buffer.
     */
    glGenBuffers(1, &m_pBuffer);

    /*
     * Activating the buffer and sending data to the GPU.
     */
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(
                GL_ARRAY_BUFFER, 
                m_positions.size()*sizeof(glm::vec3), 
                m_positions.data(), 
                GL_STATIC_DRAW
            )
    );

    /*
     * Creating the texture associated with lakes.
     */
    std::vector<std::string> filenames;
    std::string name= "../textures/shutter_texture_water_lake";
    std::string extension = ".png";
    int minRes = 2;
    int maxRes = 8; 
    for (int i = minRes; i <= maxRes; ++i) {
        filenames.push_back(name + std::to_string(i) + extension);
    }
    sendMipMapTextures(filenames, &m_textureId);
}

LakeRenderable::~LakeRenderable()
{
    /*
     * Disabling the GPU buffers used while rendering.
     */
    glcheck(glDeleteBuffers(1, &m_pBuffer));
}

void LakeRenderable::do_draw()
{
    /*
     * Gathering uniforms and attributes locations.
     */

    // Position.
    int positionLocation        = m_shaderProgram->getAttributeLocation("coord");

     // Model matrix.
    int modelLocation           = m_shaderProgram->getUniformLocation("modelMat");

    // The texture scale parameter.
    int scaleTextureLocation    = m_shaderProgram->getUniformLocation("scaleTexture");

    // Texture.
    int lakeTextureLocation     = m_shaderProgram->getUniformLocation("seaTex"); 


    /*
     * Sending material uniform to the GPU.
     */
    Material::sendToGPU(m_shaderProgram, m_material);

    /*
     * Binding position buffer.
     */
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

    /*
     * Sending the model matrix (uniform).
     */
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

    /*
     * Sending the texture scale parameter (uniform).
     */
    if (scaleTextureLocation != ShaderProgram::null_location)
    {
        glcheck(glUniform1f(scaleTextureLocation, 20.0f));
    }

    /*
     * Binding and sending the lake texture.
     */
    if (lakeTextureLocation != ShaderProgram::null_location)
	{
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_textureId));
        glcheck(glUniform1i(lakeTextureLocation, 0)); 
	}
    
    /*
     * Drawing the triangles in order to render the scene.
     */
    glcheck(glDrawArrays(GL_TRIANGLES, 0, m_positions.size()));
    
    /*
     * Releasing the texture.
     */
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));

    /*
     * Disabling the buffers.
     */
    if(positionLocation != ShaderProgram::null_location)
	{
	    glcheck(glDisableVertexAttribArray(positionLocation));
	}

}

void LakeRenderable::do_animate( float time )
{}

void LakeRenderable::setMaterial(const MaterialPtr& material) {
    m_material = material;
}
