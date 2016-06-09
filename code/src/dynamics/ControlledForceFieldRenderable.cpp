#include "./../../include/dynamics/ControlledForceFieldRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

ControlledForceFieldStatus::~ControlledForceFieldStatus(){}

ControlledForceFieldStatus::ControlledForceFieldStatus()
{
    clear();
}

ControlledForceFieldStatus::ControlledForceFieldStatus(const glm::vec3& initial_direction)
{
    clear();
    initial =  initial_direction;
    movement = initial_direction;
};

void ControlledForceFieldStatus::clear()
{
    initial = glm::vec3(0,0,0);
    movement = glm::vec3(0,0,0);
    angle =  0;
    last_time =  0;
    intensity = 0;
    acceleration = 10.0;
    deacceleration = 5.0;
    angularSpeed = 2.0;
    dampingFactor = 0.8;
    min_intensity = -1;
    max_intensity = 10;

    accelerating =  false;
    deaccelerating =  false;
    turning_left =  false;
    turning_right =  false;
}

ControlledForceFieldRenderable::ControlledForceFieldRenderable(ShaderProgramPtr program, ConstantForceFieldPtr forceField,  MapGenerator & map, Viewer & viewer)
    : HierarchicalRenderable(program), m_force( forceField ), m_map(map), m_viewer(viewer), m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    glm::vec3 initial_direction(1,0,0);
    m_status = ControlledForceFieldStatus(initial_direction);

    //Create geometric data to display an arrow representing the movement of the particle
    const std::vector<ParticlePtr>& particles = m_force->getParticles();
    m_positions.reserve(2.0*particles.size());
    m_colors.reserve(2.0*particles.size());
    m_normals.reserve(2.0*particles.size());

    for(ParticlePtr p : particles)
    {
        m_positions.push_back(p->getPosition());
        m_positions.push_back(p->getPosition() + m_status.movement);
        m_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
        m_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
        m_normals.push_back(glm::vec3(1.0,0.0,0.0));
        m_normals.push_back(glm::vec3(1.0,0.0,0.0));
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

ControlledForceFieldRenderable::~ControlledForceFieldRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

void ControlledForceFieldRenderable::do_keyPressedEvent( sf::Event& e )
{
    if( e.key.code == sf::Keyboard::Left )
    {
        m_status.turning_left = true;
    }
    else if( e.key.code == sf::Keyboard::Right )
    {
        m_status.turning_right = true;
    }
    else if( e.key.code == sf::Keyboard::Up )
    {
        m_status.accelerating = true;
    }
    else if( e.key.code == sf::Keyboard::Down )
    {
        m_status.deaccelerating = true;
    }
}

void ControlledForceFieldRenderable::do_keyReleasedEvent( sf::Event& e )
{
    if( e.key.code == sf::Keyboard::Left )
    {
        m_status.turning_left = false;
    }
    else if( e.key.code == sf::Keyboard::Right )
    {
        m_status.turning_right = false;
    }
    else if( e.key.code == sf::Keyboard::Up )
    {
        m_status.accelerating = false;
    }
    else if( e.key.code == sf::Keyboard::Down )
    {
        m_status.deaccelerating = false;
    }
}

void ControlledForceFieldRenderable::do_animate( float time )
{
    if( time > m_status.last_time )
    {
        float dt = time - m_status.last_time;

        if ( m_status.turning_left && !m_status.turning_right )
        {
            m_status.angle += dt * m_status.angularSpeed;
            float cos = std::cos( m_status.angle );
            float sin = std::sin( m_status.angle );
            m_status.movement = glm::vec3(cos * m_status.initial.x - sin * m_status.initial.y,
                                          sin * m_status.initial.x + cos * m_status.initial.y,
                                          0);
        }
        else if( m_status.turning_right && !m_status.turning_left )
        {
            m_status.angle -= dt * m_status.angularSpeed;
            float cos = std::cos( m_status.angle );
            float sin = std::sin( m_status.angle );
            m_status.movement = glm::vec3(cos * m_status.initial.x - sin * m_status.initial.y,
                                          sin * m_status.initial.x + cos * m_status.initial.y,
                                          0);
        }

        if( m_status.accelerating )
            m_status.intensity += dt * m_status.acceleration;
        else if( m_status.deaccelerating )
            m_status.intensity -= dt * m_status.deacceleration;
        else m_status.intensity *= dt * m_status.dampingFactor;

        m_status.intensity = glm::clamp( m_status.intensity, m_status.min_intensity, m_status.max_intensity );

        m_force->setForce( m_status.movement * m_status.intensity );
    }
    m_status.last_time = time;
}

void ControlledForceFieldRenderable::do_draw()
{
    //Update vertices positions from particle's positions
    const std::vector<ParticlePtr>& particles = m_force->getParticles();
    m_positions.reserve(2.0*particles.size());
    m_colors.reserve(2.0*particles.size());
    m_normals.reserve(2.0*particles.size());
    m_positions.resize(0);
    m_colors.resize(0);
    m_normals.resize(0);

    //Display an arrow representing the movement of the particle
    for(ParticlePtr p : particles)
    {
        glm::vec3 position = p->getPosition();
        position.z = m_map.getHeight(position.x, position.y) + 5.0f;

        m_positions.push_back(position);
        m_positions.push_back(position  + 2.0f* m_status.movement);
        m_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
        m_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
        m_normals.push_back(glm::vec3(1.0,0.0,0.0));
        m_normals.push_back(glm::vec3(1.0,0.0,0.0));
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
