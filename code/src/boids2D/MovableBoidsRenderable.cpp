#include "../../include/boids2D/MovableBoidsRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"
#include "../../include/Io.hpp"
#include "../../include/Utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

MovableBoidsRenderable::MovableBoidsRenderable(ShaderProgramPtr shaderProgram, BoidsManagerPtr boidsManager, BoidType boidType,
    const std::string& mesh, const std::string & texture) 
    : HierarchicalRenderable(shaderProgram),
    m_instanceVBO(0), m_VBO(0), m_texId(0),m_boidType(boidType), m_boidsManager(boidsManager)
{
    std::vector< glm::vec2 > texCoords;
    std::vector< glm::vec3 > positions;
    std::vector< glm::vec3 > normals;
    std::vector< unsigned int > indices;

    read_obj(mesh, positions, indices, normals, texCoords);

    for(unsigned int i = 0; i<indices.size(); i++) {
        m_vectorBuffer.push_back(positions[indices[i]]);
        m_vectorBuffer.push_back(glm::vec3(0.0, 0.0, 0.0));
        m_vectorBuffer.push_back(normals[indices[i]]);
        m_texCoords.push_back(texCoords[indices[i]]);
    }

    // number of element in the m_vectorBuffer (positions, colors, normals, ...)
    m_nbElement = 3;

    //Create texture
    glcheck(glGenTextures(1, &m_texId));

    //Bind the texture
    glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

    //Textured options
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    sf::Image image;
    image.loadFromFile(texture);
    image.flipVertically(); // sfml inverts the v axis... put the image in OpenGL convention: lower left corner is (0,0)
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image.getPixelsPtr()));

    //Release the texture
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));

    for(MovableBoidPtr m : m_boidsManager->getMovableBoids()){
        m_VAOs.push_back(0); 
    }

    for(unsigned int i = 0; i < m_VAOs.size(); ++i) {
        glGenVertexArrays(1, &(m_VAOs[i]));
        glcheck(glBindVertexArray(m_VAOs[i]));
    }
    
    glGenBuffers(1, &m_VBO);
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_vectorBuffer.size() * sizeof(glm::vec3), m_vectorBuffer.data(), GL_STATIC_DRAW));

    compute_modelMatrix();
    glGenBuffers(1, &m_instanceVBO);
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_modelMatrix.size() * sizeof(glm::mat4), m_modelMatrix.data(), GL_DYNAMIC_DRAW));

    glcheck(glGenBuffers(1, &m_tBuffer)); 
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW));

}

void MovableBoidsRenderable::do_draw()
{
    compute_modelMatrix();

    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_modelMatrix.size() * sizeof(glm::mat4), m_modelMatrix.data(), GL_DYNAMIC_DRAW));

    int positionLocation = m_shaderProgram->getAttributeLocation("position");
    int colorLocation = m_shaderProgram->getAttributeLocation("color");
    int normalLocation = m_shaderProgram->getAttributeLocation("normal");
    int modelLocation = m_shaderProgram->getAttributeLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    int texcoordLocation = m_shaderProgram->getAttributeLocation("texCoord");
    int texsamplerLocation = m_shaderProgram->getUniformLocation("texSampler");

    //Send material uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);

    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(modelLocation)); 
        glcheck(glVertexAttribPointer(modelLocation, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0));
        glcheck(glVertexAttribDivisor(modelLocation, 1));

        glcheck(glEnableVertexAttribArray(modelLocation + 1)); 
        glcheck(glVertexAttribPointer(modelLocation + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)( sizeof(glm::vec4) )));
        glcheck(glVertexAttribDivisor(modelLocation + 1, 1));

        glcheck(glEnableVertexAttribArray(modelLocation + 2)); 
        glcheck(glVertexAttribPointer(modelLocation + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4))));
        glcheck(glVertexAttribDivisor(modelLocation + 2, 1));

        glcheck(glEnableVertexAttribArray(modelLocation + 3)); 
        glcheck(glVertexAttribPointer(modelLocation + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4))));
        glcheck(glVertexAttribDivisor(modelLocation + 3, 1));
    }

    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0));
    }

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        glcheck(glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))));
    }

    if(normalLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float))));
    }

    if( nitLocation != ShaderProgram::null_location )
    {
        glcheck(glUniformMatrix3fv(nitLocation, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
    }

    //Bind texture in Textured Unit 0
    if(texcoordLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));
        //Send "texSampler" to Textured Unit 0
        glcheck(glUniform1i(texsamplerLocation, 0));
        glcheck(glEnableVertexAttribArray(texcoordLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
        glcheck(glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    glDrawArraysInstanced(GL_TRIANGLES, 0, m_vectorBuffer.size() / m_nbElement, m_modelMatrix.size());

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
    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(modelLocation));
        glcheck(glDisableVertexAttribArray(modelLocation + 1));
        glcheck(glDisableVertexAttribArray(modelLocation + 2));
        glcheck(glDisableVertexAttribArray(modelLocation + 3));
    }
    if(texcoordLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(texcoordLocation ));
    }
}

void MovableBoidsRenderable::do_animate(float time) {}

void MovableBoidsRenderable::compute_modelMatrix()
{
    m_modelMatrix.clear();
    std::vector<MovableBoidPtr> mvB = m_boidsManager->getMovableBoids();

    for (MovableBoidPtr m : mvB) {
        if(m->getBoidType() == m_boidType && m->toDisplay()) {
            glm::mat4 transformation(1.0);
            glm::mat4 model = getModelMatrix();
            
            float cz = cos(m->getAngle() - M_PI / 2.0);
            float sz = sin(m->getAngle() - M_PI / 2.0);
            float cy;
            float sy;
            if(m->isDead()) {
                cy = cos(M_PI / 2.0);
                sy = sin(M_PI / 2.0);  
            } else {
                cy = cos(0.0);
                sy = sin(0.0); 
            }
            
            float cx = cos(- M_PI / 2.0);
            float sx = sin(- M_PI / 2.0);

            float scale = m->getScale();
            if(m->getBoidType() == RABBIT){
                scale /= 8.0;
            } else if(m->getBoidType() == WOLF) {
                scale *= 2.0;
            }
             
            glm::vec3 position = m->getLocation();
            
            transformation[0][0] = scale * cz * cy;
            transformation[0][1] = scale * sz * cy;
            transformation[0][2] = - scale * sy;

            transformation[1][0] = scale * (-sz * cx + sx * cz * sy);
            transformation[1][1] = scale * (cz * cx + sx * sz * sy);
            transformation[1][2] = scale * (-sx * cy) ;
            
            transformation[2][0] = scale * (sz * sx + cz * cx * sy);
            transformation[2][1] = scale * (-sx * cz + sz * sy * cx);
            transformation[2][2] = scale * (cy * cx);

            transformation[3][0] = position.x;
            transformation[3][1] = position.y;
            transformation[3][2] = position.z;
            transformation[3][3] = 1;

            m_modelMatrix.push_back(model * transformation);
        }
    }
}

MovableBoidsRenderable::~MovableBoidsRenderable()
{
    glcheck(glDeleteBuffers(1, &m_instanceVBO));
    for(unsigned int i = 0; i < m_VAOs.size(); ++i) {
        glcheck(glDeleteBuffers(1, &(m_VAOs[i])));
    }
    glcheck(glDeleteBuffers(1, &m_VBO));
    glcheck(glDeleteBuffers(1, &m_tBuffer));
    glcheck(glDeleteBuffers(1, &m_texId));
}

void MovableBoidsRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
