#include "./../../include/boids2D/SightRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"
#include "./../../include/boids2D/MovableBoid.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


SightRenderable::SightRenderable(ShaderProgramPtr shaderProgram, MovableBoidPtr boid)
  : HierarchicalRenderable(shaderProgram), m_boid(boid), m_display(false),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    double startAngle = boid->getAngle() - boid->getParameters()->getAngleView() / 2.0;
    double endAngle = boid->getAngle() + boid->getParameters()->getAngleView() / 2.0;

    double numberFace = 16;
    double incr = M_PI/numberFace;

    double ptheta = startAngle;
    double ntheta = ptheta + incr;
    double pcos = cos(ptheta);
    double psin = sin(ptheta);
    double ncos, nsin;

    while(ptheta < endAngle){

        ncos = cos(ntheta);
        nsin = sin(ntheta);

        m_positions.push_back( glm::vec3(pcos, psin, 0.0) );
        m_positions.push_back( glm::vec3(ncos, nsin, 0.0) );
        m_positions.push_back( glm::vec3(0.0, 0.0, 0.0) );

        for(int i = 0; i < 3; i++) {
            m_normals.push_back( glm::vec3(0.0, 0.0, 1.0) );
            m_colors.push_back(glm::vec4(1.0, 0.0, 0.0, 1.0));
        }

        ptheta = ntheta;
        ntheta += incr;
        pcos = ncos;
        psin = nsin;
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

void SightRenderable::do_draw()
{
    if(!m_display) 
    {
        return;
    }
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

    glm::mat4 transformation(1.0);
    glm::mat4 model = getModelMatrix();

    float c = cos(m_boid->getAngle());
    float s = sin(m_boid->getAngle());
    float scale = m_boid->getParameters()->getDistViewMax();

    glm::vec3 position = m_boid->getLocation();
    //position.z = m_boidsManager->getHeight(position.x, position.y) + 1.0;

    transformation[0][0] = scale * c;
    transformation[0][1] = scale * s;

    transformation[1][1] = scale * c;
    transformation[1][0] = scale * -s;
    
    transformation[2][2] = scale;
    transformation[3][0] = position.x;
    transformation[3][1] = position.y;
    transformation[3][2] = position.z - 0.1f;
    transformation[3][3] = 1;

    glcheck(glUniformMatrix4fv( modelLocation, 1, GL_FALSE, glm::value_ptr(model * transformation)));

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

void SightRenderable::do_animate(float time) {}

void SightRenderable::do_keyPressedEvent( sf::Event& e)
{
    if(e.key.code == sf::Keyboard::O)
    {
        m_display = !m_display;
    }
}

SightRenderable::~SightRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}
