#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "./../../include/ShaderProgram.hpp"
#include "./../../include/log.hpp"
#include "./../../include/gl_helper.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>

/**
 * @brief Material properties of an object for the Phong illumination model.
 *
 * This class represents the material properties of an object for
 * the illumination model of Phong.
 */
class Material
{
public:
    typedef std::shared_ptr<Material> MaterialPtr; /*!< Smart pointer to a material */

    /**
     * @brief Destructor
     */
    ~Material();

    /**
     * @brief Default constructor
     */
    Material();

    /**
     * @brief Default constructor
     */
    Material(const Material& material);

    /**
     * @brief Specific constructor
     *
     * Construct a material
     *
     * @param ambient The ambient vector of the material.
     * @param diffuse The diffuse vector of the material.
     * @param specular The specular vector of the material.
     * @param shininess The shininess coefficient of the material.
     */
    Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float& shininess);

    /**
     * @brief Access to the ambient vector of the material.
     *
     * @return A const reference to m_ambient.
     */
    const glm::vec3& ambient() const;

    /**
     * @brief Set the ambient vector of the material.
     *
     * Set the value of m_ambient.
     * @param ambient The new ambient vector of the material.
     */
    void setAmbient(const glm::vec3 &ambient);

    /**
     * @brief Access to the diffuse vector of the material.
     *
     * @return A const reference to m_diffuse.
     */
    const glm::vec3& diffuse() const;

    /**
     * @brief Set the diffuse vector of the material.
     *
     * Set the value of m_diffuse.
     * @param diffuse The new diffuse vector of the material.
     */
    void setDiffuse(const glm::vec3 &diffuse);

    /**
     * @brief Access to the specular vector of the material.
     *
     * @return A const reference to m_specular.
     */
    const glm::vec3& specular() const;

    /**
     * @brief Set the specular vector of the material.
     *
     * Set the value of m_specular.
     * @param specular The new specular vector of the material.
     */
    void setSpecular(const glm::vec3 &specular);

    /**
     * @brief Access to the shininess coefficient of the material.
     *
     * @return A const reference to m_shininess.
     */
    const float& shininess() const;

    /**
     * @brief Set the shininess coefficient of the material.
     *
     * Set the value of m_shininess.
     * @param shininess The new shininess vector of the material.
     */
    void setShininess(float shininess);

    /**
     * @brief Get location for the attributes of the material and send the data to the GPU as uniforms.
     *
     * @param program A pointer to the shader program where to get the locations.
     * @param material A pointer to the material to send to the GPU.
     * @return  True if everything was fine, false otherwise
     */
    static bool sendToGPU(const ShaderProgramPtr& program, const MaterialPtr& material);

    /**
     * @brief Construct a pearl material from real data according to http://devernay.free.fr/cours/opengl/materials.html
     * @return A pearl material.
     */
    static MaterialPtr Pearl();

    /**
     * @brief Construct a emerald material from real data according to http://devernay.free.fr/cours/opengl/materials.html
     * @return A emerald material.
     */
    static MaterialPtr Emerald();

    /**
     * @brief Construct a bronze material from real data according to http://devernay.free.fr/cours/opengl/materials.html
     * @return A bronze material.
     */
    static MaterialPtr Bronze();

private:
    glm::vec3 m_ambient; /*!< The ambient material vector defines what color this object reflects under ambient lighting. */
    glm::vec3 m_diffuse; /*!< The diffuse material vector defines the color of the object under diffuse lighting. */
    glm::vec3 m_specular; /*!< The specular material vector sets the color impact a specular light has on the object. */
    float m_shininess; /*!< The shininess impacts the scattering/radius of the specular highlight. */
};

typedef std::shared_ptr<Material> MaterialPtr; /*!< Smart pointer to a material */

#endif //MATERIAL_HPP
