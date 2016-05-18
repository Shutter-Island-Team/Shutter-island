/* Created on: Mar 7, 2016
 *     Author: T.Delame (tdelame@gmail.com)
 */
#include "../../include/dynamics/ParticleListRenderable.hpp"
#include <glm/gtc/type_ptr.hpp>

ParticleListRenderable::ParticleListRenderable(ShaderProgramPtr program, std::vector<ParticlePtr>& particles)
    : HierarchicalRenderable( program ),
      m_positionBuffer( 0 )
{
    for(ParticlePtr p : particles)
    {
        m_particles.push_back(p);
    }

    std::vector< glm::vec3 > positions;
    std::vector< glm::vec3 > normals;
    std::vector< glm::vec4 > colors;
    getUnitSphere( positions, normals, 20, 20 );

    m_numberOfVertices = positions.size();
    colors.resize( m_numberOfVertices );
    for( size_t i = 0; i < m_numberOfVertices; ++ i )
    {
        colors[i] = getColor( positions[i].x, -1, 1 );
    }

    glcheck( glGenBuffers( 1, &m_positionBuffer ));
    glcheck( glGenBuffers( 1, &m_colorBuffer ));
    glcheck( glGenBuffers( 1, &m_normalBuffer ));

    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, 0 ));
}

void ParticleListRenderable::do_draw()
{
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer));
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if( colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if( normalLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    const size_t nparticles = m_particles.size();
    glm::mat4 model = getModelMatrix();
    glm::mat4 transformation(1.0);
    for( size_t i = 0; i < nparticles; ++ i ) {
    	ParticlePtr particlePtr = m_particles[i];
    	if (particlePtr->isRounded()) {
    	    RoundedParticlePtr roundedPtr = (RoundedParticlePtr) std::static_pointer_cast<RoundedParticle, Particle>(particlePtr);
    	    glm::vec3 position = roundedPtr->getPosition();
    	    float scale = roundedPtr->getRadius();
    	    transformation[0][0] = scale;
    	    transformation[1][1] = scale;
    	    transformation[2][2] = scale;
    	    transformation[3][0] = position.x;
    	    transformation[3][1] = position.y;
    	    transformation[3][2] = position.z;
    	} else {
    	    continue;
    	}

        glcheck(glUniformMatrix4fv( modelLocation, 1, GL_FALSE, glm::value_ptr(model * transformation)));
        glcheck(glDrawArrays(GL_TRIANGLES, 0, m_numberOfVertices));
    }
}


void ParticleListRenderable::do_animate( float time )
{}

ParticleListRenderable::~ParticleListRenderable()
{
    glcheck(glDeleteBuffers(1, &m_positionBuffer));
    glcheck(glDeleteBuffers(1, &m_colorBuffer));
    glcheck(glDeleteBuffers(1, &m_normalBuffer));
}
