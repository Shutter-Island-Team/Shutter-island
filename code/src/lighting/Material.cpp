#include "./../../include/lighting/Material.hpp"
#include <glm/gtc/type_ptr.hpp>

Material::~Material()
{}

Material::Material()
{
    m_ambient = glm::vec3(0.0,0.0,0.0);
    m_diffuse = glm::vec3(0.0,0.0,0.0);
    m_specular = glm::vec3(0.0,0.0,0.0);
    m_shininess = 0.0;
}

Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float &shininess)
{
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_shininess = shininess;
}

Material::Material(const Material& material)
{
    m_ambient = material.m_ambient;
    m_diffuse = material.m_diffuse;
    m_specular = material.m_specular;
    m_shininess = material.m_shininess;
}

const glm::vec3& Material::ambient() const
{
    return m_ambient;
}

void Material::setAmbient(const glm::vec3 &ambient)
{
    m_ambient = ambient;
}

const glm::vec3& Material::diffuse() const
{
    return m_diffuse;
}

void Material::setDiffuse(const glm::vec3 &diffuse)
{
    m_diffuse = diffuse;
}

const glm::vec3& Material::specular() const
{
    return m_specular;
}

void Material::setSpecular(const glm::vec3 &specular)
{
    m_specular = specular;
}

void Material::setShininess(float shininess)
{
    m_shininess = shininess;
}

const float &Material::shininess() const
{
    return m_shininess;
}

bool Material::sendToGPU(const ShaderProgramPtr& program, const MaterialPtr &material)
{
    bool success = true;
    int location = -1;

    if(program==nullptr || material==nullptr)
    {
        return false;
    }

    location = program->getUniformLocation("material.ambient");
    if(location!=ShaderProgram::null_location)
    {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(material->ambient())));
    }
    else
    {
        success = false;
    }

    location = program->getUniformLocation("material.diffuse");
    if(location!=ShaderProgram::null_location)
    {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(material->diffuse())));
    }
    else
    {
        success = false;
    }

    location = program->getUniformLocation("material.specular");
    if(location!=ShaderProgram::null_location)
    {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(material->specular())));
    }
    else
    {
        success = false;
    }

    location = program->getUniformLocation("material.shininess");
    if(location!=ShaderProgram::null_location)
    {
        glcheck(glUniform1f(location, material->shininess()));
    }
    else
    {
        success = false;
    }

    return success;
}

MaterialPtr Material::Pearl()
{
    float openGLFactor=128.0;
    glm::vec3 ambient(0.25, 0.20725, 0.20725);
    glm::vec3 diffuse(1.0, 0.829, 0.829);
    glm::vec3 specular(0.296648, 0.296648, 0.296648);
    float shininess = openGLFactor*0.088;
    return std::make_shared<Material>(ambient, diffuse, specular, shininess);
}

MaterialPtr Material::Emerald()
{
    float openGLFactor=128.0;
    glm::vec3 ambient(0.0215, 0.1745, 0.0215);
    glm::vec3 diffuse(0.07568, 0.61424, 0.07568);
    glm::vec3 specular(0.633, 0.727811, 0.633);
    float shininess = openGLFactor*0.6;
    return std::make_shared<Material>(ambient, diffuse, specular, shininess);
}

MaterialPtr Material::Bronze()
{
    float openGLFactor=128.0;
    glm::vec3 ambient(0.2125, 0.1275, 0.054);
    glm::vec3 diffuse(0.714, 0.4284, 0.18144);
    glm::vec3 specular(0.393548, 0.271906, 0.166721);
    float shininess = openGLFactor*0.2;
    return std::make_shared<Material>(ambient, diffuse, specular, shininess);
}
