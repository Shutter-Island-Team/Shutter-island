#include "./../../include/dynamics/KartWheelRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

KartWheelRenderable::KartWheelRenderable(ShaderProgramPtr shaderProgram, RoundedParticlePtr particle, int nb) :
    HierarchicalRenderable(shaderProgram),
    m_particle(particle),
    m_pBuffer(0),
    m_cBuffer(0),
    m_nBuffer(0)
{
    // for (double theta = 0.0 ; theta < M_PI * 2.0; theta += (M_PI * 2.0) / nb ) {
    //     m_positions.push_back(glm::vec3(cos(theta), 0.0, sin(theta)));
    //     m_positions.push_back(glm::vec3(cos(theta + (M_PI * 2.0) / nb), 0.0, sin(theta + (M_PI * 2.0) / nb)));
    //     m_positions.push_back(glm::vec3(cos(theta), 1.0, sin(theta)));
        
    //     m_positions.push_back(glm::vec3(cos(theta + (M_PI * 2.0) / nb), 0.0, sin(theta + (M_PI * 2.0) / nb)));
    //     m_positions.push_back(glm::vec3(cos(theta), 0.0, sin(theta)));
    //     m_positions.push_back(glm::vec3(0, 0, 0));
        
    //     m_positions.push_back(glm::vec3(cos(theta + (M_PI * 2.0) / nb), 0.0, sin(theta + (M_PI * 2.0) / nb)));
    //     m_positions.push_back(glm::vec3(cos(theta + (M_PI * 2.0) / nb), 1.0, sin(theta + (M_PI * 2.0) / nb)));
    //     m_positions.push_back(glm::vec3(cos(theta), 1.0, sin(theta)));
        
    //     m_positions.push_back(glm::vec3(cos(theta + (M_PI * 2.0) / nb), 1.0, sin(theta + (M_PI * 2.0) / nb)));
    //     m_positions.push_back(glm::vec3(cos(theta), 1.0, sin(theta)));
    //     m_positions.push_back(glm::vec3(0, 1, 0));
        
    //     m_colors.push_back(randomColor());
    //     m_colors.push_back(randomColor());
    //     m_colors.push_back(randomColor());
        
    //     glm::vec3 norm_1(0.0, 1.0, 0.0);
    //     glm::vec4 color = glm::vec4(norm_1.x, norm_1.y, norm_1.z, 1.0);
    //     m_colors.push_back(color);
    //     m_colors.push_back(color);
    //     m_colors.push_back(color);
        
    //     m_colors.push_back(randomColor());
    //     m_colors.push_back(randomColor());
    //     m_colors.push_back(randomColor());
        
    //     m_colors.push_back(color);
    //     m_colors.push_back(color);
    //     m_colors.push_back(color);
    // }

    // //Create buffers
    // glGenBuffers(1, &m_pBuffer); //vertices
    // glGenBuffers(1, &m_cBuffer); //colors
    // glGenBuffers(1, &m_nBuffer); //normals

    // //Activate buffer and send data to the graphics card
    // glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    // glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    // glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    // glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    // glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    // glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));

    glm::vec3 center(0.0,0.0,0.0);

    std::vector<glm::vec2> tmp_tex;
    getUnitCube(m_positions, m_normals, tmp_tex);

    //Assign one color to each face
    m_colors.push_back(glm::vec4(1,0,0,1));
    m_colors.push_back(glm::vec4(1,0,0,1));
    m_colors.push_back(glm::vec4(1,0,0,1));

    m_colors.push_back(glm::vec4(0,1,0,1));
    m_colors.push_back(glm::vec4(0,1,0,1));
    m_colors.push_back(glm::vec4(0,1,0,1));

    m_colors.push_back(glm::vec4(0,0,1,1));
    m_colors.push_back(glm::vec4(0,0,1,1));
    m_colors.push_back(glm::vec4(0,0,1,1));

    m_colors.push_back(glm::vec4(0,1,1,1));
    m_colors.push_back(glm::vec4(0,1,1,1));
    m_colors.push_back(glm::vec4(0,1,1,1));

    m_colors.push_back(glm::vec4(1,0,1,1));
    m_colors.push_back(glm::vec4(1,0,1,1));
    m_colors.push_back(glm::vec4(1,0,1,1));

    m_colors.push_back(glm::vec4(1,1,0,1));
    m_colors.push_back(glm::vec4(1,1,0,1));
    m_colors.push_back(glm::vec4(1,1,0,1));

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

void KartWheelRenderable::do_draw()
{
    //Update the parent and local transform matrix to position the geometric data according to the particle's data.
    const float& pRadius = m_particle->getRadius();
    const glm::vec3& pPosition = m_particle->getPosition();
    glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(pRadius));
    glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(pPosition));
    setLocalTransform(translate*scale);

    //Draw geometric data
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
  

    //Send material uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);

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
     if( nitLocation != ShaderProgram::null_location )
    {
	glcheck(glUniformMatrix3fv( nitLocation, 1, GL_FALSE,
	 glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
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

void KartWheelRenderable::do_animate(float time) {}

KartWheelRenderable::~KartWheelRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

void KartWheelRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
