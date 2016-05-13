#include "./../include/FrameRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

/* NOTE: remember to always use the glcheck macro for gl commands.
 */

FrameRenderable::~FrameRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
}

FrameRenderable::FrameRenderable(ShaderProgramPtr shaderProgram)
  // This ":" symbol start a constructor initialization section.
  // It is a good practice to initialize data and acquire resources
  // here. It faster to perform initialization here than in the
  // constructor body.
  : Renderable(shaderProgram), // call the Renderable constructor
    m_pBuffer(0), m_cBuffer(0) // initialize the buffers to default value (useful for debugging purposes)
{
    initAttributes();

    glcheck(glGenBuffers(1, &m_pBuffer)); //vertices
    glcheck(glGenBuffers(1, &m_cBuffer)); //colors

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
}

void FrameRenderable::initAttributes()
{
    m_positions.push_back( glm::vec3(0,0,0) );
    m_positions.push_back( glm::vec3(1,0,0) );
    m_positions.push_back( glm::vec3(0,0,0) );
    m_positions.push_back( glm::vec3(0,1,0) );
    m_positions.push_back( glm::vec3(0,0,0) );
    m_positions.push_back( glm::vec3(0,0,1) );

    m_colors.push_back( glm::vec4(1,0,0,1) );
    m_colors.push_back( glm::vec4(1,0,0,1) );
    m_colors.push_back( glm::vec4(0,1,0,1) );
    m_colors.push_back( glm::vec4(0,1,0,1) );
    m_colors.push_back( glm::vec4(0,0,1,1) );
    m_colors.push_back( glm::vec4(0,0,1,1) );
}

void FrameRenderable::do_draw()
{
    // get the uniform and attribute locations on the shader program. Remember,
    // we do not need to bind the shader program to perform such actions for 2
    // reasons:
    //  - those locations are stored in a look up table in the c++ side
    //  - we are in do_draw(), the shader program is guaranteed to be bound
    GLint mLoc = m_shaderProgram->getUniformLocation("modelMat");
    GLint vLoc = m_shaderProgram->getAttributeLocation("vPosition");
    GLint cLoc = m_shaderProgram->getAttributeLocation("vColor");


    //Change width of line primitive to 3 pixels
    glcheck(glLineWidth(3.0f));

    //Send uniform to the GPU
    glcheck(glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));

    //Bind vertices position location
    glcheck(glEnableVertexAttribArray(vLoc));
    //Bind vertices position buffer
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    //Link vertices position location to vertices position buffer data
    glcheck(glVertexAttribPointer(vLoc, 3, GL_FLOAT, GL_FALSE, 0, (void *)0));

    //Bind vertices color location
    glcheck(glEnableVertexAttribArray(cLoc));
    //Bind vertices color buffer
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    //Link vertices color location to vertices color buffer data
    glcheck(glVertexAttribPointer(cLoc, 4, GL_FLOAT, GL_FALSE, 0, (void *)0));

    //Draw lines elements
    glcheck(glDrawArrays(GL_LINES, 0, m_positions.size()));

    // Note: this is a good practice, at least in our case, to restore the
    // openGL state to some default state. If you don't, you can have two kind
    // of issues:
    //   - you do not know why the rendering does not produce anymore what you
    //   expect in other parts of your code, e.g. you deactivate the blending here
    //   and are looking for why another renderable that needs blending is not rendering
    //   properly anymore
    //
    //   - silent errors, e.g. in another part of your code you do not bind all
    //   necessary buffers. Thus the graphic pipeline will read buffers used for other
    //   renderable and no errors will be displayed. If you unbind all buffers, an
    //   error will be produced by the drawing command exactly where you forgot something.

    glcheck(glDisableVertexAttribArray(vLoc));
    glcheck(glDisableVertexAttribArray(cLoc));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, 0 ));
    glcheck(glLineWidth(1.0f));
}

void FrameRenderable::do_animate(float time)
{}
