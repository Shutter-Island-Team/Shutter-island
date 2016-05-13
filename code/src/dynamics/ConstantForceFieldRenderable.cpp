#include "../../include/dynamics/ConstantForceFieldRenderable.hpp"
#include "../../include/log.hpp"
#include "../../include/gl_helper.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

ConstantForceFieldRenderable::ConstantForceFieldRenderable(ShaderProgramPtr shaderProgram, ConstantForceFieldPtr forceField) :
    HierarchicalRenderable(shaderProgram),
    m_forceField(forceField),
    m_pBuffer(0),
    m_cBuffer(0)
{
    //Create geometric data
    const std::vector<ParticlePtr>& particles = m_forceField->getParticles();
    m_positions.resize(2.0*particles.size());
    m_colors.resize(2.0*particles.size());
    m_normals.resize(2.0*particles.size());

    int i=0; 
    for(ParticlePtr p : particles)
    {
        m_positions[2*i+0] = p->getPosition();
        m_positions[2*i+1] = p->getPosition() + 0.1f*m_forceField->getForce();
        m_colors[2*i+0] = glm::vec4(1.0,0.0,0.0,1.0);
        m_colors[2*i+1] = glm::vec4(1.0,0.0,0.0,1.0);
        m_normals[2*i+0] = glm::vec3(1.0,0.0,0.0);
        m_normals[2*i+1] = glm::vec3(1.0,0.0,0.0);
        i++;
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

void ConstantForceFieldRenderable::do_draw()
{
    //Update vertices positions from particle's positions
    const std::vector<ParticlePtr>& particles = m_forceField->getParticles();
    m_positions.resize(2.0*particles.size());
    m_colors.resize(2.0*particles.size());
    m_normals.resize(2.0*particles.size());
    int i=0; 
    for(ParticlePtr p : particles)
    {
        m_positions[2*i+0] = p->getPosition();
        m_positions[2*i+1] = p->getPosition() + 0.1f*m_forceField->getForce();
        m_colors[2*i+0] = glm::vec4(1.0,0.0,0.0,1.0);
        m_colors[2*i+1] = glm::vec4(1.0,0.0,0.0,1.0);
        m_normals[2*i+0] = glm::vec3(1.0,0.0,0.0);
        m_normals[2*i+1] = glm::vec3(1.0,0.0,0.0);
        i++;
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

    //Draw elements
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

void ConstantForceFieldRenderable::do_animate(float time) {}

ConstantForceFieldRenderable::~ConstantForceFieldRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}
