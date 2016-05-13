#include "./../../include/dynamics/SpringListRenderable.hpp"
#include "../../include/log.hpp"
#include "../../include/gl_helper.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

SpringListRenderable::SpringListRenderable(ShaderProgramPtr shaderProgram, std::list<SpringForceFieldPtr>& springForceFields) :
    HierarchicalRenderable(shaderProgram),
    m_springForceFields(springForceFields),
    m_pBuffer(0),
    m_cBuffer(0)
{
    //Create geometric data
    size_t springNumber =  m_springForceFields.size();
    m_positions.resize(2*springNumber);
    m_colors.resize(2*springNumber, glm::vec4(0.0,0.0,1.0,1.0));
    m_normals.resize(2*springNumber, glm::vec3(1.0,1.0,1.0));
    int counter=0;
    for(SpringForceFieldPtr s : m_springForceFields)
    {
        m_positions[2*counter+0] = s->getParticle1()->getPosition();
        m_positions[2*counter+1] = s->getParticle2()->getPosition();
        counter++;
    }

    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_nBuffer); //normals

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
}

void SpringListRenderable::do_draw()
{
    //Update vertices positions from particle's positions
    size_t springNumber =  m_springForceFields.size();
    m_positions.resize(2*springNumber);
    m_colors.resize(2*springNumber, glm::vec4(0.0,0.0,1.0,1.0));
    m_normals.resize(2*springNumber, glm::vec3(1.0,1.0,1.0));
    int counter=0;
    for(SpringForceFieldPtr s : m_springForceFields)
    {
        m_positions[2*counter+0] = s->getParticle1()->getPosition();
        m_positions[2*counter+1] = s->getParticle2()->getPosition();
        counter++;
    }

    //Update data on the GPU
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));


    //Draw geometric data
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
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

    //Draw triangles elements
    glLineWidth(3.0);
    glcheck(glDrawArrays(GL_LINES,0, m_positions.size()));
    glLineWidth(1.0);

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
}

void SpringListRenderable::do_animate(float time) {}

SpringListRenderable::~SpringListRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}
