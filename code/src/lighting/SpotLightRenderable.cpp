#include "./../../include/lighting/SpotLightRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

SpotLightRenderable::SpotLightRenderable(ShaderProgramPtr shaderProgram, SpotLightPtr light) :
    HierarchicalRenderable(shaderProgram), m_light(light),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    std::vector<glm::vec3> tmp_x, tmp_n;
    unsigned int strips=20, slices=20;

    getUnitCone(tmp_x, tmp_n, strips, slices);
    m_positions.insert(m_positions.end(), tmp_x.begin(), tmp_x.end());
    m_normals.insert(m_normals.end(), tmp_n.begin(), tmp_n.end());
    m_colors.resize(m_positions.size(), glm::vec4(light->diffuse(),1.0));

    //Translate to light position
    glm::mat4 translation(1.0), rotation(1.0), scale(1.0), transformation(1.0);
    translation = glm::translate(glm::mat4(1.0), m_light->position());

    //Scale according to the spot angle
    float scaleFactor = std::tan(std::acos(m_light->outerCutOff()))*1.0f;
    scale = glm::scale(glm::mat4(1.0), glm::vec3(scaleFactor, scaleFactor,1.0));

    //Orientate along spot direction
    glm::vec3 init_direction(0.0,0.0,1.0);
    glm::quat quat = glm::rotation(init_direction, m_light->spotDirection());
    rotation = glm::toMat4(quat)*glm::rotate(glm::mat4(1.0), (float)M_PI, glm::vec3(1.0,0.0,0.0));

    transformation = translation*rotation*scale;
    setParentTransform(transformation);

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

void SpotLightRenderable::do_draw()
{
    //Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    //Send data to GPU
    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
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

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

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

void SpotLightRenderable::do_animate(float /*time*/) {}

SpotLightRenderable::~SpotLightRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}
