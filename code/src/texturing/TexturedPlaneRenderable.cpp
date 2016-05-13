#include "./../../include/texturing/TexturedPlaneRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

TexturedPlaneRenderable::~TexturedPlaneRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteTextures(1, &m_texId));
}

TexturedPlaneRenderable::TexturedPlaneRenderable(ShaderProgramPtr shaderProgram, const std::string& filename)
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_cBuffer(0), m_nBuffer(0), m_tBuffer(0), m_wrapOption(0), m_filterOption(0)
{
    //Initialize geometry
    getUnitPlane(m_positions, m_normals, m_origTexCoords);
    m_texCoords = m_origTexCoords;
    m_colors.resize(m_positions.size(), glm::vec4(1.0,1.0,1.0,1.0));

    //Create texture
    glGenTextures(1, &m_texId);

    //Bind the texture
    glBindTexture(GL_TEXTURE_2D, m_texId);

    //Textured options
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    sf::Image image;
    image.loadFromFile(filename);
    image.flipVertically(); // sfml inverts the v axis... put the image in OpenGL convention: lower left corner is (0,0)
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image.getPixelsPtr()));

    //Release the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_nBuffer); //normals
    glGenBuffers(1, &m_tBuffer); //texture coords

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

void TexturedPlaneRenderable::do_draw()
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

void TexturedPlaneRenderable::do_animate(float time)
{

}

void TexturedPlaneRenderable::updateTextureOption()
{
    //Resize texture coordinates factor
    float factor=10.0;

    std::string text;

    //Bind the texture
    glBindTexture(GL_TEXTURE_2D, m_texId);

    //Textured options
    switch(m_wrapOption)
    {
    case 0:
        for(size_t i=0; i<m_texCoords.size(); ++i)
        {
            m_texCoords[i] = m_origTexCoords[i];
        }
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        text.append("Wrapping: CLAMP_TO_EDGE");
        break;

    case 1:
        for(size_t i=0; i<m_texCoords.size(); ++i)
        {
            m_texCoords[i] = factor*m_origTexCoords[i];
        }
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        text.append("Wrapping: REPEAT");
        break;

    case 2:
        for(size_t i=0; i<m_texCoords.size(); ++i)
        {
            m_texCoords[i] = factor*m_origTexCoords[i];
        }
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
        text.append("Wrapping: MIRRORED_REPEAT");
        break;

    case 3:
        for(size_t i=0; i<m_texCoords.size(); ++i)
        {
            m_texCoords[i] = factor*m_origTexCoords[i] - glm::vec2(factor/2.0, factor/2.0);
        }
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        text.append("Wrapping: CLAMP_TO_EDGE");
        break;

    case 4:
        for(size_t i=0; i<m_texCoords.size(); ++i)
        {
            m_texCoords[i] = factor*m_origTexCoords[i] - glm::vec2(factor/2.0, factor/2.0);
        }
        float borderColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
        glcheck(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        text.append("Wrapping: CLAMP_TO_BORDER");
        break;
    }


    switch(m_filterOption)
    {
    case 0:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        text.append(", Filtering: NEAREST");
        break;

    case 1:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        text.append(", Filtering: LINEAR");
        break;

//    case 2:
//        // generate mipmap images
//        glcheck(glGenerateMipmap(GL_TEXTURE_2D));
//        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
//        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
//        text.append(", Filtering: MIPMAP");
//        break;
    }

    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords.size()*sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW));

    //Release the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    displayTextInViewer(text);
}

void TexturedPlaneRenderable::do_keyPressedEvent( sf::Event& e )
{
    if(e.key.code == sf::Keyboard::F6)
    {
        m_wrapOption = m_wrapOption+1 > 4 ? 0 : m_wrapOption+1;
        updateTextureOption();
    }
    if(e.key.code == sf::Keyboard::F7)
    {
        m_filterOption = m_filterOption+1 > 1 ? 0 : m_filterOption+1;
        updateTextureOption();
    }
}

void TexturedPlaneRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
