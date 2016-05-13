#include "./../include/TriangleRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

TriangleRenderable::TriangleRenderable(ShaderProgramPtr shaderProgram) : Renderable(shaderProgram)
{
    m_positions.push_back(glm::vec3(-1, 0, 0));
    m_positions.push_back(glm::vec3(1, 0, 0));
    m_positions.push_back(glm::vec3(0, 1, 0));

    m_colors.push_back(glm::vec4(1,0,0,1));
    m_colors.push_back(glm::vec4(1,0,0,1));
    m_colors.push_back(glm::vec4(1,0,0,1));

    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
}

void TriangleRenderable::do_draw()
{
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));

    glcheck(glEnableVertexAttribArray(positionLocation));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

    glcheck(glEnableVertexAttribArray(colorLocation));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    glcheck(glDisableVertexAttribArray(positionLocation));
    glcheck(glDisableVertexAttribArray(colorLocation));
}

void TriangleRenderable::do_animate(float time) {}

TriangleRenderable::~TriangleRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
}
