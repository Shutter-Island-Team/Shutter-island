#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "./../../include/ShaderProgram.hpp"

/**
 * @brief A directional light.
 *
 * This class represents a directional light which means a light
 * whose source is so far away from the camera that the light rays
 * can be considered close to parallel from each other regardless of
 * the position of the objects or the camera. A good example is the
 * sun.
 */
class DirectionalLight
{
public:

    typedef std::shared_ptr<DirectionalLight> DirectionalLightPtr; /*!< Smart pointer to a directional light */

    /**
     * @brief Destructor
     */
    ~DirectionalLight();

    /**
     * @brief Default constructor
     */
    DirectionalLight();

    /**
     * @brief Copy constructor
     */
    DirectionalLight(const DirectionalLight& directionalLight);

    /**
    * @brief Specific constructor
    *
    * Construct a directional light.
    *
    * @param direction The direction of the light.
    * @param ambient The ambient intensity of the light.
    * @param diffuse The diffuse intensity of the light.
    * @param specular The specular intensity of the light.
    */
    DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

    /**
     * @brief Get location for the attributes of the light and send the data to the GPU as uniforms.
     *
     * @param program A pointer to the shader program where to get the locations.
     * @param light A pointer to the light to send to the GPU.
     * @return  True if everything was fine, false otherwise
     */
    static bool sendToGPU(const ShaderProgramPtr& program, const DirectionalLightPtr& light);

    /**
     * @brief Access to the direction of the light.
     *
     * @return A const reference to m_direction.
     */
    const glm::vec3& direction() const;

    /**
     * @brief Set the direction of the light.
     *
     * Set the value of m_direction.
     * @param direction The new direction of the light.
     */
    void setDirection(const glm::vec3 &direction);

    /**
     * @brief Access to the ambient intensity of the light.
     *
     * @return A const reference to m_ambient.
     */
    const glm::vec3 &ambient() const;

    /**
     * @brief Set the ambient intensity of the light.
     *
     * Set the value of m_ambient.
     * @param ambient The new ambient intensity of the light.
     */
    void setAmbient(const glm::vec3 &ambient);

    /**
     * @brief Access to the diffuse intensity of the light.
     *
     * @return A const reference to m_diffuse.
     */
    const glm::vec3& diffuse() const;

    /**
     * @brief Set the diffuse intensity of the light.
     *
     * Set the value of m_diffuse.
     * @param diffuse The new diffuse intensity of the light.
     */
    void setDiffuse(const glm::vec3 &diffuse);

    /**
     * @brief Access to the specular intensity of the light.
     *
     * @return A const reference to m_specular.
     */
    const glm::vec3 &specular() const;

    /**
     * @brief Set the specular intensity of the light.
     *
     * Set the value of m_specular.
     * @param specular The new specular intensity of the light.
     */
    void setSpecular(const glm::vec3 &specular);

private:    
    glm::vec3 m_direction;  /*!< The direction of the light. */
    glm::vec3 m_ambient;    /*!< Intensity of the light with respect to the object ambient components. */
    glm::vec3 m_diffuse;    /*!< Intensity of the light with respect to the object diffuse components. */
    glm::vec3 m_specular;   /*!< Intensity of the light with respect to the object specular components. */

    /**
     * @brief Get the uniform name of the light.
     *
     * @return The name of the light in the shader.
     */
    std::string lightName() { return std::string("directionalLight"); }

    /**
     * @brief Get the uniform name of the direction of the light.
     *
     * @return The name of the direction of the light in the shader.
     */
    std::string directionName() { return std::string("direction"); }

    /**
     * @brief Get the uniform name of the ambient intensity of the light.
     *
     * @return The name of the ambient intensity of the light in the shader.
     */
    std::string ambientName() { return std::string("ambient"); }

    /**
     * @brief Get the uniform name of the diffuse intensity of the light.
     *
     * @return The name of the diffuse intensity of the light in the shader.
     */
    std::string diffuseName() { return std::string("diffuse"); }

    /**
     * @brief Get the uniform name of the specular intensity of the light.
     *
     * @return The name of the specular intensity of the light in the shader.
     */
    std::string specularName() { return std::string("specular"); }
};

typedef std::shared_ptr<DirectionalLight> DirectionalLightPtr; /*!< Smart pointer to a directional light */

/**
 * @brief A point light.
 *
 * This class represents a point light which means a light
 * with a given position somewhere in a world that illuminates
 * in all directions. Good examples of point light are torches.
 */
class PointLight
{
public:
    typedef std::shared_ptr<PointLight> PointLightPtr; /*!< Smart pointer to a point light */

    /**
     * @brief Destructor
     */
    ~PointLight();

    /**
     * @brief Default constructor
     */
    PointLight();

    /**
     * @brief Default constructor
     */
    PointLight(const PointLight& pointLight);

    /**
     * @brief Specific constructor
     *
     * Construct a point light.
     *
     * @param position The position of the light.
     * @param ambient The ambient intensity of the light.
     * @param diffuse The diffuse intensity of the light.
     * @param specular The specular intensity of the light.
     * @param constant The coefficient of constant attenuation of the light.
     * @param linear The coefficient of linear attenuation of the light with respect to the distance to the light.
     * @param quadratic The coefficient of quadratic attenuation of the light with respect to the distance to the light.
     */
    PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
               const float& constant, const float& linear, const float& quadratic);

    /**
     * @brief Get location for the attributes of the light and send the data to the GPU as uniforms.
     *
     * @param program A pointer to the shader program where to get the locations.
     * @param light A pointer to the light to send to the GPU.
     * @return  True if everything was fine, false otherwise
     */
    static bool sendToGPU(const ShaderProgramPtr& program, const PointLightPtr& light);

    /**
     * @brief Get location for the attributes of the lights and send the data to the GPU as uniforms.
     *
     * @param program A pointer to the shader program where to get the locations.
     * @param lights A vector of pointer to the lights to send to the GPU.
     * @return  True if everything was fine, false otherwise
     */
    static bool sendToGPU(const ShaderProgramPtr& program, const std::vector<PointLightPtr>& lights);

    /**
     * @brief Access to the position of the light.
     *
     * @return A const reference to m_position.
     */
    const glm::vec3 &position() const;

    /**
     * @brief Set the position of the light.
     *
     * Set the value of m_position.
     * @param position The new position of the light.
     */
    void setPosition(const glm::vec3 &position);

    /**
     * @brief Access to the ambient intensity of the light.
     *
     * @return A const reference to m_ambient.
     */
    const glm::vec3& ambient() const;

    /**
     * @brief Set the ambient intensity of the light.
     *
     * Set the value of m_ambient.
     * @param ambient The new ambient intensity of the light.
     */
    void setAmbient(const glm::vec3 &ambient);

    /**
     * @brief Access to the diffuse intensity of the light.
     *
     * @return A const reference to m_diffuse.
     */
    const glm::vec3 &diffuse() const;

    /**
     * @brief Set the diffuse intensity of the light.
     *
     * Set the value of m_diffuse.
     * @param diffuse The new diffuse intensity of the light.
     */
    void setDiffuse(const glm::vec3 &diffuse);

    /**
     * @brief Access to the specular intensity of the light.
     *
     * @return A const reference to m_specular.
     */
    const glm::vec3& specular() const;

    /**
     * @brief Set the specular intensity of the light.
     *
     * Set the value of m_specular.
     * @param specular The new specular intensity of the light.
     */
    void setSpecular(const glm::vec3 &specular);

    /**
     * @brief Access to the coefficient of constant attenuation of the light.
     *
     * @return A const reference to m_constant.
     */
    const float &constant() const;

    /**
     * @brief Set the coefficient of constant attenuation of the light.
     *
     * Set the value of m_constant.
     * @param constant The new coefficient of constant attenuation of the light.
     */
    void setConstant(float constant);

    /**
     * @brief Access to the coefficient of linear attenuation of the light.
     *
     * @return A const reference to m_linear.
     */
    const float& linear() const;

    /**
     * @brief Set the coefficient of linear attenuation of the light.
     *
     * Set the value of m_linear.
     * @param linear The new coefficient of linear attenuation of the light.
     */
    void setLinear(float linear);

    /**
     * @brief Access to the coefficient of quadratic attenuation of the light.
     *
     * @return A const reference to m_quadratic.
     */
    const float &quadratic() const;

    /**
     * @brief Set the coefficient of quadratic attenuation of the light.
     *
     * Set the value of m_quadratic.
     * @param quadratic The new coefficient of quadratic attenuation of the light.
     */
    void setQuadratic(float quadratic);

private:
    glm::vec3 m_position; /*!< The position of the light. */

    glm::vec3 m_ambient;    /*!< Intensity of the light with respect to the object ambient components. */
    glm::vec3 m_diffuse;    /*!< Intensity of the light with respect to the object diffuse components. */
    glm::vec3 m_specular;   /*!< Intensity of the light with respect to the object specular components. */

    float m_constant;       /*!< Coefficient of constant attenuation of the light. */
    float m_linear;         /*!< Coefficient of linear attenuation of the light with respect to the distance to the light position. */
    float m_quadratic;      /*!< Coefficient of quadratic attenuation of the light with respect to the distance to the light position. */

    /**
     * @brief Get the uniform name of the number of lights.
     *
     * @return The name of the number of lights in the shader.
     */
    std::string numberOfLightsName() { return std::string("numberOfPointLight"); }

    /**
     * @brief Get the uniform name of the light.
     *
     * @return The name of the light in the shader.
     */
    std::string lightName() { return std::string("pointLight"); }

    /**
     * @brief Get the uniform name of the position of the light.
     *
     * @return The name of the position of the light in the shader.
     */
    std::string positionName() { return std::string("position"); }


    /**
     * @brief Get the uniform name of the ambient intensity of the light.
     *
     * @return The name of the ambient intensity of the light in the shader.
     */
    std::string ambientName() { return std::string("ambient"); }

    /**
     * @brief Get the uniform name of the diffuse intensity of the light.
     *
     * @return The name of the diffuse intensity of the light in the shader.
     */
    std::string diffuseName() { return std::string("diffuse"); }

    /**
     * @brief Get the uniform name of the specular intensity of the light.
     *
     * @return The name of the specular intensity of the light in the shader.
     */
    std::string specularName() { return std::string("specular"); }

    /**
     * @brief Get the uniform name of the constant attenuation coefficient of the light.
     *
     * @return The name of the constant attenuation coefficient of the light in the shader.
     */
    std::string constantName() { return std::string("constant"); }

    /**
     * @brief Get the uniform name of the linear attenuation coefficient of the light.
     *
     * @return The name of the linear attenuation coefficient of the light in the shader.
     */
    std::string linearName() { return std::string("linear"); }

    /**
     * @brief Get the uniform name of the quadratic attenuation coefficient of the light.
     *
     * @return The name of the quadratic attenuation coefficient of the light in the shader.
     */
    std::string quadraticName() { return std::string("quadratic"); }
};

typedef std::shared_ptr<PointLight> PointLightPtr; /*!< Smart pointer to a point light */

/**
 * @brief A spot light.
 *
 * This class represents a spot light which means a light
 * whose source is located somewhere in the environment and
 * instead of shooting light rays in all directions,
 * only shoots them in a specific direction.
 * As a result, only the objects within a certain radius
 * of the spotlight's direction are lit and everything else stays dark.
 * A good example of a spotlight would be a street lamp.
 */
class SpotLight
{
public:
    typedef std::shared_ptr<SpotLight> SpotLightPtr; /*!< Smart pointer to a spot light */

    /**
     * @brief Destructor
     */
    ~SpotLight();

    /**
     * @brief Default constructor
     */
    SpotLight();

    /**
     * @brief Copy constructor
     */
    SpotLight(const SpotLight& spotLight);

    /**
    * @brief Specific constructor
    *
    * Construct a spot light.
    *
    * @param position The position of the light.
    * @param spotDirection The direction of the light.
    * @param ambient The ambient intensity of the light.
    * @param diffuse The diffuse intensity of the light.
    * @param specular The specular intensity of the light.
    * @param constant The coefficient of constant attenuation of the light.
    * @param linear The coefficient of linear attenuation of the light with respect to the distance to the light.
    * @param quadratic The coefficient of quadratic attenuation of the light with respect to the distance to the light.
    * @param innerCutOff The cosinus of the inner cut off angle of the spot.
    * @param outerCutOff The cosinus of the outer cut off angle of the spot.
    */
    SpotLight(const glm::vec3& position, const glm::vec3& spotDirection,
              const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
              const float& constant, const float& linear, const float& quadratic,
              const float& innerCutOff, const float& outerCutOff);

    /**
     * @brief Get location for the attributes of the light and send the data to the GPU as uniforms.
     *
     * @param program A pointer to the shader program where to get the locations.
     * @param light A pointer to the light to send to the GPU.
     * @return  True if everything was fine, false otherwise
     */
    static bool sendToGPU(const ShaderProgramPtr& program, const SpotLightPtr& light);

    /**
     * @brief Get location for the attributes of the lights and send the data to the GPU as uniforms.
     *
     * @param program A pointer to the shader program where to get the locations.
     * @param lights A vector of pointer to the lights to send to the GPU.
     * @return  True if everything was fine, false otherwise
     */
    static bool sendToGPU(const ShaderProgramPtr& program, const std::vector<SpotLightPtr>& lights);

    /**
     * @brief Access to the position of the light.
     *
     * @return A const reference to m_position.
     */
    const glm::vec3 &position() const;

    /**
     * @brief Set the position of the light.
     *
     * Set the value of m_position.
     * @param position The new position of the light.
     */
    void setPosition(const glm::vec3 &position);

    /**
     * @brief Access to the direction of the spot.
     *
     * @return A const reference to m_direction.
     */
    const glm::vec3 &spotDirection() const;

    /**
     * @brief Set the direction of the spot.
     *
     * Set the value of m_direction.
     * @param spotDirection The new direction of the light.
     */
    void setSpotDirection(const glm::vec3 &spotDirection);

    /**
     * @brief Access to the ambient intensity of the light.
     *
     * @return A const reference to m_ambient.
     */
    const glm::vec3& ambient() const;

    /**
     * @brief Set the ambient intensity of the light.
     *
     * Set the value of m_ambient.
     * @param ambient The new ambient intensity of the light.
     */
    void setAmbient(const glm::vec3 &ambient);

    /**
     * @brief Access to the diffuse intensity of the light.
     *
     * @return A const reference to m_diffuse.
     */
    const glm::vec3 &diffuse() const;

    /**
     * @brief Set the diffuse intensity of the light.
     *
     * Set the value of m_diffuse.
     * @param diffuse The new diffuse intensity of the light.
     */
    void setDiffuse(const glm::vec3 &diffuse);

    /**
     * @brief Access to the specular intensity of the light.
     *
     * @return A const reference to m_specular.
     */
    const glm::vec3& specular() const;

    /**
     * @brief Set the specular intensity of the light.
     *
     * Set the value of m_specular.
     * @param specular The new specular intensity of the light.
     */
    void setSpecular(const glm::vec3 &specular);

    /**
     * @brief Access to the coefficient of constant attenuation of the light.
     *
     * @return A const reference to m_constant.
     */
    const float &constant() const;

    /**
     * @brief Set the coefficient of constant attenuation of the light.
     *
     * Set the value of m_constant.
     * @param constant The new coefficient of constant attenuation of the light.
     */
    void setConstant(float constant);

    /**
     * @brief Access to the coefficient of linear attenuation of the light.
     *
     * @return A const reference to m_linear.
     */
    const float& linear() const;

    /**
     * @brief Set the coefficient of linear attenuation of the light.
     *
     * Set the value of m_linear.
     * @param linear The new coefficient of linear attenuation of the light.
     */
    void setLinear(float linear);

    /**
     * @brief Access to the coefficient of quadratic attenuation of the light.
     *
     * @return A const reference to m_quadratic.
     */
    const float &quadratic() const;

    /**
     * @brief Set the coefficient of quadratic attenuation of the light.
     *
     * Set the value of m_quadratic.
     * @param quadratic The new coefficient of quadratic attenuation of the light.
     */
    void setQuadratic(float quadratic);

    /**
     * @brief Access to the cosinus of the inner cut off angle of the spot.
     *
     * @return A const reference to m_innerCutOff.
     */
    const float& innerCutOff() const;

    /**
     * @brief Set the cosinus of the inner cut off angle of the spot.
     *
     * Set the value of m_innerCutOff.
     * @param innerCutOff The new cosinus of the inner cut off angle of the spot.
     */
    void setInnerCutOff(float innerCutOff);

    /**
     * @brief Access to the cosinus of the outer cut off angle of the spot.
     *
     * @return A const reference to m_outerCutOff.
     */
    const float &outerCutOff() const;

    /**
     * @brief Set the cosinus of the outer cut off angle of the spot.
     *
     * Set the value of m_outerCutOff.
     * @param outerCutOff The new cosinus of the outer cut off angle of the spot.
     */
    void setOuterCutOff(float outerCutOff);

private:
    glm::vec3 m_position; /*!< The position of the light. */
    glm::vec3 m_spotDirection; /*!< The direction of the spot. */

    glm::vec3 m_ambient;    /*!< Intensity of the light with respect to the object ambient components. */
    glm::vec3 m_diffuse;    /*!< Intensity of the light with respect to the object diffuse components. */
    glm::vec3 m_specular;   /*!< Intensity of the light with respect to the object specular components. */

    float m_constant;       /*!< Coefficient of constant attenuation of the light. */
    float m_linear;         /*!< Coefficient of linear attenuation of the light with respect to the distance to the light position. */
    float m_quadratic;      /*!< Coefficient of quadratic attenuation of the light with respect to the distance to the light position. */

    float m_innerCutOff;    /*!< The cosinus of the inner cutoff angle that specifies the spotlight's inner radius. Everything inside this angle is fully lit by the spotlight. */
    float m_outerCutOff;    /*!< The cosinus of the outer cutoff angle that specifies the spotlight's outer radius. Everything outside this angle is not lit by the spotlight. */

    /**
     * @brief Get the uniform name of the number of lights.
     *
     * @return The name of the number of lights in the shader.
     */
    std::string numberOfLightsName() { return std::string("numberOfSpotLight"); }

    /**
     * @brief Get the uniform name of the light.
     *
     * @return The name of the light in the shader.
     */
    std::string lightName() { return std::string("spotLight"); }

    /**
     * @brief Get the uniform name of the position of the light.
     *
     * @return The name of the position of the light in the shader.
     */
    std::string positionName() { return std::string("position"); }

    /**
     * @brief Get the uniform name of the direction of the spot.
     *
     * @return The name of the direction of the spot in the shader.
     */
    std::string spotDirectionName() { return std::string("spotDirection"); }

    /**
     * @brief Get the uniform name of the ambient intensity of the light.
     *
     * @return The name of the ambient intensity of the light in the shader.
     */
    std::string ambientName() { return std::string("ambient"); }

    /**
     * @brief Get the uniform name of the diffuse intensity of the light.
     *
     * @return The name of the diffuse intensity of the light in the shader.
     */
    std::string diffuseName() { return std::string("diffuse"); }

    /**
     * @brief Get the uniform name of the specular intensity of the light.
     *
     * @return The name of the specular intensity of the light in the shader.
     */
    std::string specularName() { return std::string("specular"); }

    /**
     * @brief Get the uniform name of the constant attenuation coefficient of the light.
     *
     * @return The name of the constant attenuation coefficient of the light in the shader.
     */
    std::string constantName() { return std::string("constant"); }

    /**
     * @brief Get the uniform name of the linear attenuation coefficient of the light.
     *
     * @return The name of the linear attenuation coefficient of the light in the shader.
     */
    std::string linearName() { return std::string("linear"); }

    /**
     * @brief Get the uniform name of the quadratic attenuation coefficient of the light.
     *
     * @return The name of the quadratic attenuation coefficient of the light in the shader.
     */
    std::string quadraticName() { return std::string("quadratic"); }

    /**
     * @brief Get the uniform name of the inner cut off cosinus angle of the spot.
     *
     * @return The name of the cosinus of the inner cut off angle of the spot in the shader.
     */
    std::string innerCutOffName() { return std::string("innerCutOff"); }

    /**
     * @brief Get the uniform name of the cosinus of the outer cut off angle of the spot.
     *
     * @return The name of the the cosinus of the outer cut off cosinus angle of the spot in the shader.
     */
    std::string outerCutOffName() { return std::string("outerCutOff"); }
};

typedef std::shared_ptr<SpotLight> SpotLightPtr; /*!< Smart pointer to a spot light */

#endif //LIGHT_HPP
