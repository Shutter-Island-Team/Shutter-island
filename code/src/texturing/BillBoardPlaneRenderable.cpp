#include "./../../include/texturing/BillBoardPlaneRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

BillBoardPlaneRenderable::~BillBoardPlaneRenderable()
{
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer));
    glcheck(glDeleteTextures(1, &m_texId));
}

static const glm::vec2 shift[4] = {
    glm::vec2{-0.5,-0.5},
    glm::vec2{ 0.5,-0.5},
    glm::vec2{ 0.5, 0.5},
    glm::vec2{-0.5, 0.5},
};

BillBoardPlaneRenderable::BillBoardPlaneRenderable(
    ShaderProgramPtr shaderProgram,
    const std::string& texture_filename,
    const glm::vec3 &billboardLocalPosition, const glm::vec2 &billboardLocalDimension)
    : Renderable(shaderProgram),
      m_cBuffer(0), m_tBuffer(0),
      m_billboardWorldPosition(billboardLocalPosition), m_billboardWorldDimension(billboardLocalDimension)
{
    // reserve enough place and initialize data with default values
    m_shift.resize( 6 );
    m_colors.resize( 6, glm::vec4(1.0,1.0, 1.0, 1.0 ) );
    m_shift[0] = shift[0];
    m_shift[1] = shift[1];
    m_shift[2] = shift[2];

    m_shift[3] = shift[0];
    m_shift[4] = shift[2];
    m_shift[5] = shift[3];

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
    image.loadFromFile(texture_filename);
    image.flipVertically(); // sfml inverts the v axis... put the image in OpenGL convention: lower left corner is (0,0)
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image.getPixelsPtr()));

    //Release the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //Create buffers
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_tBuffer); //texture coords

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_shift.size()*sizeof(glm::vec2), m_shift.data(), GL_STATIC_DRAW));
}

void BillBoardPlaneRenderable::do_draw()
{
    //Location
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int shiftLocation = m_shaderProgram->getAttributeLocation("vShift");
    int texSampleLoc = m_shaderProgram->getUniformLocation("texSampler");
    int billboardPositionLocation = m_shaderProgram->getUniformLocation("billboard_world_position");
    int billboardDimensionsLocation = m_shaderProgram->getUniformLocation("billboard_world_dimensions");

    //Send material uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);

    //Send uniform to the graphics card
    if( billboardPositionLocation != ShaderProgram::null_location )
    {
        glcheck(glUniform3fv( billboardPositionLocation, 1, glm::value_ptr( m_billboardWorldPosition ) ) );
    }
    if( billboardDimensionsLocation != ShaderProgram::null_location )
    {
        glcheck(glUniform2fv( billboardDimensionsLocation, 1, glm::value_ptr( m_billboardWorldDimension) ) );
    }
    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Bind texture in Textured Unit 0
    if(shiftLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));
        //Send "texSampler" to Textured Unit 0
        glcheck(glUniform1i(texSampleLoc, 0));
        glcheck(glEnableVertexAttribArray(shiftLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
        glcheck(glVertexAttribPointer(shiftLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, 6));

    //Release texture
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));
    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
    if(shiftLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(shiftLocation));
    }
}

void BillBoardPlaneRenderable::do_animate(float time)
{}

void BillBoardPlaneRenderable::do_keyPressedEvent( sf::Event& e )
{}

void BillBoardPlaneRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
