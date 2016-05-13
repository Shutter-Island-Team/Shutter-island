# include "../include/KeyframedCylinderRenderable.hpp"
# include "../include/gl_helper.hpp"
# include "../include/GeometricTransformation.hpp"
# include "../include/Utils.hpp"

# include <glm/gtc/type_ptr.hpp>
# include <GL/glew.h>

KeyframedCylinderRenderable::KeyframedCylinderRenderable( ShaderProgramPtr prog )
   : HierarchicalRenderable( prog )
 {
    unsigned int numberOfSlice=50;
    getUnitCylinder(m_positions, m_normals, numberOfSlice);
    unsigned int numberOfVertices = m_positions.size();
    m_colors.resize(numberOfVertices);
    for(size_t i=0; i<numberOfVertices/3; i++)
    {
        float factor=240.0*(float)3*i/(float)numberOfVertices;
        m_colors[3*i] = getColor(factor, 0.0, 240.0);
        m_colors[3*i+1] = getColor(factor, 0.0, 240.0);
        m_colors[3*i+2] = getColor(factor, 0.0, 240.0);
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

void KeyframedCylinderRenderable::addLocalTransformKeyframe( const GeometricTransformation& transformation, float time )
{
  m_localKeyframes.add( transformation, time );
}

void KeyframedCylinderRenderable::addParentTransformKeyframe( const GeometricTransformation& transformation, float time )
{
  m_parentKeyframes.add( transformation, time );
}

void KeyframedCylinderRenderable::do_draw()
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

void KeyframedCylinderRenderable::do_animate( float time )
{
    //Assign the interpolated transformations from the keyframes to the local/parent transformations.
    if(!m_localKeyframes.empty())
    {
        setLocalTransform( m_localKeyframes.interpolateTransformation( time ) );
    }
    if(!m_parentKeyframes.empty())
    {
        setParentTransform( m_parentKeyframes.interpolateTransformation( time ) );
    }
}

KeyframedCylinderRenderable::~KeyframedCylinderRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}
