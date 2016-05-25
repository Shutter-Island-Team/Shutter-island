#include "./../../include/boids2D/StateRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

StateRenderable::StateRenderable(ShaderProgramPtr shaderProgram, MovableBoidPtr boid) :
    HierarchicalRenderable(shaderProgram), m_display(false),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    m_boid = boid;
    m_savedState = boid->getStateType();
    glm::vec4 color = getColorFromState();
    
    double startAngle = 0.0;
    double endAngle = 2.0 * M_PI;

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

        m_positions.push_back( glm::vec3(pcos, psin, -0.2) );
        m_positions.push_back( glm::vec3(ncos, nsin, -0.2) );
        m_positions.push_back( glm::vec3(0.0, 0.0, -0.2) );

        for(int i = 0; i < 3; i++) {
            m_normals.push_back( glm::vec3(0.0, 0.0, 1.0) );
            m_colors.push_back(color);
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
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_DYNAMIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
}

void StateRenderable::do_draw()
{
    if (!m_display) 
    {
        return;
    }

    if (m_savedState != m_boid->getStateType()) 
    {
        m_savedState = m_boid->getStateType();

        glm::vec4 color = getColorFromState();

        for (size_t i = 0; i<m_colors.size(); ++i)
        {
            m_colors[i] = color;
        }

        //Update color buffer and send data to the graphics card
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_DYNAMIC_DRAW));
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

    glm::vec3 position = m_boid->getLocation();
    transformation[0][0] = 1;
    transformation[1][1] = 1;
    transformation[2][2] = 0.1f;
    transformation[3][0] = position.x;
    transformation[3][1] = position.y;
    transformation[3][2] = position.z - 0.05f;
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

void StateRenderable::do_animate(float time) {}

StateRenderable::~StateRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

void StateRenderable::do_keyPressedEvent( sf::Event& e )
{
    if(e.key.code == sf::Keyboard::P) {
        m_display = !m_display;
    }
}

glm::vec4 StateRenderable::getColorFromState()
{
    glm::vec4 color;
    switch (m_savedState) {
        case WALK_STATE:
            // Red
            color = glm::vec4(0.95, 0.26, 0.21, 1.0);
            break;
        case STAY_STATE:
            // Indigo
            color = glm::vec4(0.24, 0.31, 0.70, 1.0);
            break;
        case FIND_FOOD_STATE:
            // Green
            color = glm::vec4(0.29, 0.68, 0.31, 1.0);
            break;
        case ATTACK_STATE:
            // Orange
            color = glm::vec4(1.0, 0.59, 0.0, 1.0);
            break;
        case EAT_STATE:
            // Brown
            color = glm::vec4(0.47, 0.33, 0.28, 1.0);
            break;
        case LOST_STATE:
            // Gray
            color = glm::vec4(0.61, 0.61, 0.61, 1.0);
            break;
        case SLEEP_STATE:
            // Yellow
            color = glm::vec4(1.0, 0.92, 0.23, 1.0);
            break;
        case FLEE_STATE:
            // Deep Purple
            color = glm::vec4(0.40, 0.22, 0.71, 1.0);
            break;
        case FIND_WATER_STATE:
            // Teal
            color = glm::vec4(0.0, 0.58, 0.53, 1.0);
            break;
        case DRINK_STATE:
            // Light Blue
            color = glm::vec4(0.01, 0.66, 0.95, 1.0);
            break;
        case MATE_STATE:
            // Pink
            color = glm::vec4(0.91, 0.11, 0.38, 1.0);
            break;
        default:
            // Black
            color = glm::vec4(0.0, 0.0, 0.0, 1.0);
            break;
    }

    return color;
}
