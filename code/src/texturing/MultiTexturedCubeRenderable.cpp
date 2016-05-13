#include "./../../include/texturing/MultiTexturedCubeRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

MultiTexturedCubeRenderable::~MultiTexturedCubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer1));
    glcheck(glDeleteBuffers(1, &m_tBuffer2));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteTextures(1, &m_texId1));
    glcheck(glDeleteTextures(1, &m_texId2));
}

MultiTexturedCubeRenderable::MultiTexturedCubeRenderable(ShaderProgramPtr shaderProgram, const std::string& filename1, const std::string &filename2)
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_cBuffer(0), m_nBuffer(0), m_tBuffer1(0), m_tBuffer2(0), m_blendingCoefficient(0.5f)
{
    //Initialize geometry
    std::vector<glm::vec2> tmp_texCoords;
    getUnitCube(m_positions, m_normals, tmp_texCoords);
    m_texCoords1 = tmp_texCoords;
    m_texCoords2 = tmp_texCoords;
    m_colors.resize(m_positions.size(), glm::vec4(1.0,1.0,1.0,1.0));

    sf::Image image;

    //Create texture
    glGenTextures(1, &m_texId1);
    //Bind the texture
    glBindTexture(GL_TEXTURE_2D, m_texId1);
    //Textured options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    image.loadFromFile(filename1);
    image.flipVertically(); // sfml inverts the v axis... put the image in OpenGL convention: lower left corner is (0,0)
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image.getPixelsPtr()));
    //Release the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //Create texture
    glGenTextures(1, &m_texId2);
    //Bind the texture
    glBindTexture(GL_TEXTURE_2D, m_texId2);
    //Textured options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    image.loadFromFile(filename2);
    image.flipVertically(); // sfml inverts the v axis... put the image in OpenGL convention: lower left corner is (0,0)
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image.getPixelsPtr()));
    //Release the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_nBuffer); //normals
    glGenBuffers(1, &m_tBuffer1); //texture coords
    glGenBuffers(1, &m_tBuffer2); //texture coords

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));

    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer1));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords1.size()*sizeof(glm::vec2), m_texCoords1.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer2));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords2.size()*sizeof(glm::vec2), m_texCoords2.data(), GL_STATIC_DRAW));
}

void MultiTexturedCubeRenderable::do_draw()
{
    //Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    int blendingCoeffLocation = m_shaderProgram->getUniformLocation("blendingCoeff");
    int textureLocation1 = m_shaderProgram->getAttributeLocation("vTexCoord1");
    int textureLocation2 = m_shaderProgram->getAttributeLocation("vTexCoord2");
    int texSampleLoc1 = m_shaderProgram->getUniformLocation("texSampler1");
    int texSampleLoc2 = m_shaderProgram->getUniformLocation("texSampler2");

    //Send material uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);

    //Send uniform to the graphics card
    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if( nitLocation != ShaderProgram::null_location )
    {
        glcheck(glUniformMatrix3fv( nitLocation, 1, GL_FALSE,
                                    glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
    }

    if(blendingCoeffLocation != ShaderProgram::null_location)
    {
        glcheck(glUniform1f(blendingCoeffLocation, m_blendingCoefficient));
    }

    if(positionLocation != ShaderProgram::null_location)
    {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(normalLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Bind texture in Textured Unit 0
    if(textureLocation1 != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId1));
        //Send "texSampler" to Textured Unit 0
        glcheck(glUniform1i(texSampleLoc1, 0));
        glcheck(glEnableVertexAttribArray(textureLocation1));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer1));
        glcheck(glVertexAttribPointer(textureLocation1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Bind texture in Textured Unit 1
    if(textureLocation2 != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE1));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId2));
        //Send "texSampler" to Textured Unit 1
        glcheck(glUniform1i(texSampleLoc2, 1));
        glcheck(glEnableVertexAttribArray(textureLocation2));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer2));
        glcheck(glVertexAttribPointer(textureLocation2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    //Release texture
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));

    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
    if(normalLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
    if(textureLocation1 != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(textureLocation1));
    }
    if(textureLocation2 != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(textureLocation2));
    }
}

void MultiTexturedCubeRenderable::do_animate(float time)
{
    m_blendingCoefficient = std::sin(time);
}

void MultiTexturedCubeRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
