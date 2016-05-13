#include "./../../include/texturing/MipMapCubeRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

MipMapCubeRenderable::~MipMapCubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteTextures(1, &m_texId)); // even with several subimages, there is still a single texture!
}

MipMapCubeRenderable::MipMapCubeRenderable(ShaderProgramPtr shaderProgram, std::vector<std::string> &filenames)
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_cBuffer(0), m_nBuffer(0), m_tBuffer(0), m_mipmapOption(0)
{
    //Initialize geometry
    getUnitCube(m_positions, m_normals, m_texCoords);
    m_colors.resize(m_positions.size(), glm::vec4(1.0,1.0,1.0,1.0));

    //Create texture
    glGenTextures(1, &m_texId);

    //Bind the texture
    glBindTexture(GL_TEXTURE_2D, m_texId);

    //Texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //Load images from files
    std::vector< sf::Image > images;
    sf::Vector2u imageSize(0,0);
    images.resize(filenames.size());
    for(int i=0; i<images.size(); ++i)
    {
        images[i].loadFromFile(filenames[i]);
        images[i].flipVertically(); // sfml inverts the v axis... put the image in OpenGL convention: lower left corner is (0,0)
        if(i==0) imageSize = images[i].getSize();
    }

    //Send the image to OpenGL as textures
    glTexStorage2D(GL_TEXTURE_2D, images.size(), GL_RGBA32F, imageSize.x, imageSize.y);
    for(int i=0; i<images.size(); ++i)
    {
        glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, images[i].getSize().x, images[i].getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)images[i].getPixelsPtr());
    }

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

void MipMapCubeRenderable::do_draw()
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

    //Bind texture in Texture Unit 0
    if(textureLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));
        //Send "texSampler" to Texture Unit 0
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

void MipMapCubeRenderable::do_animate(float time)
{

}

void MipMapCubeRenderable::updateTextureOption()
{
    std::string text;

    //Bind the texture
    glBindTexture(GL_TEXTURE_2D, m_texId);

    // Here multiple texture files are loaded
    // Otherwise, generate multiresolution images with:
    //     glcheck(glGenerateMipmap(GL_TEXTURE_2D));

    //Textured options
    switch(m_mipmapOption)
    {
    case 0:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        text.append("Filtering: LINEAR_MIPMAP_LINEAR");
        break;

    case 1:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        text.append("Filtering: LINEAR_MIPMAP_NEAREST");
        break;

    case 2:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        text.append("Filtering: NEAREST_MIPMAP_NEAREST");
        break;

    case 3:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        text.append("Filtering: NEAREST_MIPMAP_LINEAR");
        break;

    }

    //Release the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    displayTextInViewer(text);
}

void MipMapCubeRenderable::do_keyPressedEvent( sf::Event& e )
{
    if(e.key.code == sf::Keyboard::F8)
    {
        m_mipmapOption = m_mipmapOption+1 > 3 ? 0 : m_mipmapOption+1;
        updateTextureOption();
    }
}


void MipMapCubeRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
