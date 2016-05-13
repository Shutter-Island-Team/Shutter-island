#include "./../../include/dynamics/KartChassisRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>



KartChassisRenderable::KartChassisRenderable(ShaderProgramPtr shaderProgram, StickParticlePtr particle) :
    HierarchicalRenderable(shaderProgram),
    m_particle(particle) 
{
    // Debug Begin
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


    // Debug end
}

void KartChassisRenderable::do_draw()
{
    // Debug begin
    //Update the parent and local transform matrix to position the geometric data according to the particle's data.
    const glm::vec3& pPosition = m_particle->getPosition();
    glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(0.5));
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

    // Debug end
}


void KartChassisRenderable::do_animate(float time) {}

KartChassisRenderable::~KartChassisRenderable()
{
    // Debug begin
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    // Debug end
}

void KartChassisRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}


