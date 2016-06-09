#include "./../../include/dynamics/ParticleCamera.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

ParticleForceFieldStatus::~ParticleForceFieldStatus(){}

ParticleForceFieldStatus::ParticleForceFieldStatus()
{
    clear();
}

ParticleForceFieldStatus::ParticleForceFieldStatus(const glm::vec3& initial_direction)
{
    clear();
    initial =  initial_direction;
    movement = initial_direction;
};

void ParticleForceFieldStatus::clear()
{
    initial = glm::vec3(0,0,0);
    movement = glm::vec3(0,0,0);
    angle =  0;
    last_time =  0;
    intensity = 0;
    acceleration = 50000.0;
    deacceleration = 50000.0;
    angularSpeed = 2.0;
    dampingFactor = 0.0;
    min_intensity = -500;
    max_intensity = 500;

    accelerating =  false;
    deaccelerating =  false;
    turning_left =  false;
    turning_right =  false;
}

ParticleCamera::ParticleCamera(ShaderProgramPtr program, ConstantForceFieldPtr forceField,  MapGenerator & map, Viewer & viewer)
    : HierarchicalRenderable(program), m_force( forceField ), m_map(map), m_viewer(viewer)
{
    glm::vec3 initial_direction(1,0,0);
    m_status = ParticleForceFieldStatus(initial_direction);
}

ParticleCamera::~ParticleCamera()
{
}

void ParticleCamera::do_keyPressedEvent( sf::Event& e )
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

void ParticleCamera::do_keyReleasedEvent( sf::Event& e )
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

void ParticleCamera::do_animate( float time )
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

void ParticleCamera::do_draw()
{
    //Update vertices positions from particle's positions
    const std::vector<ParticlePtr>& particles = m_force->getParticles();

    glm::vec3 position;
    if(particles.size() != 0) {
        position = particles[0]->getPosition();
        position.z = m_map.getHeight(position.x, position.y) + 5.0f;
    }
    
    if (particles.size() != 0 && m_viewer.getCamera().isFollowingParticle()) {
        float ecartY = 6.0;
        float angleCamera = m_status.angle - M_PI/2.0;
        glm::vec3 cercle = glm::vec3(ecartY*sin(angleCamera), ecartY*(1-cos(angleCamera)), 0.0);
        m_viewer.getCamera().setViewMatrix( glm::lookAt( position + glm::vec3(0.2, -ecartY, 4.0) + cercle, position + glm::vec3(0.0, 0.0, 2.0), glm::vec3( 0, 0, 1 ) ) );
    }
}
