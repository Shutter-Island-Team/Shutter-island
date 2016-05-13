#include "./../../include/texturing/TexturedCubeRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

TexturedCubeRenderable::~TexturedCubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteTextures(1, &m_texId));
}

TexturedCubeRenderable::TexturedCubeRenderable(ShaderProgramPtr shaderProgram, const std::string& filename)
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_cBuffer(0), m_nBuffer(0), m_tBuffer(0)
{
    //Initialize geometry
    getUnitCube(m_positions, m_normals, m_texCoords);
    m_colors.resize(m_positions.size(), glm::vec4(1.0,1.0,1.0,1.0));

    //Create texture
    glcheck(glGenTextures(1, &m_texId));

    //Bind the texture
    glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

    //Textured options
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    sf::Image image;
    image.loadFromFile(filename);
    image.flipVertically(); // sfml inverts the v axis... put the image in OpenGL convention: lower left corner is (0,0)
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image.getPixelsPtr()));

    //Release the texture
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));

    //Create buffers
    glcheck(glGenBuffers(1, &m_pBuffer)); //vertices
    glcheck(glGenBuffers(1, &m_cBuffer)); //colors
    glcheck(glGenBuffers(1, &m_nBuffer)); //normals
    glcheck(glGenBuffers(1, &m_tBuffer)); //texture coords

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords.size()*sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW));
}

void TexturedCubeRenderable::do_draw()
{
    //Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
        int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    int textureLocation = m_shaderProgram->getAttributeLocation("vTexCoord");
    int texSampleLoc = m_shaderProgram->getUniformLocation("texSampler");

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
    if(textureLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));
        //Send "texSampler" to Textured Unit 0
        glcheck(glUniform1i(texSampleLoc, 0));
        glcheck(glEnableVertexAttribArray(textureLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
        glcheck(glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
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
    if(textureLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(textureLocation));
    }
}

void TexturedCubeRenderable::do_animate(float time)
{

}

void TexturedCubeRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
