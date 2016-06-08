/**
 * @file LakeRenderable.hpp
 *
 * @brief 
 * Simply a renderable for the lakes.
 * Some treatments are done so as to take into account
 * connexe lakes (by mergeing them into one lake).
 */

#ifndef LAKERENDERABLE_HPP
#define LAKERENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "MapGenerator.hpp"

#include <list>
#include <utility>
#include <vector>
#include <glm/glm.hpp>

/**
 * @brief LakeRenderable
 */
class LakeRenderable : public HierarchicalRenderable
{
public :

    /**
     * @brief Destructor
     */
    ~LakeRenderable();

    /**
     * @brief Constructor
     *
     * @param shaderProgram Shader program used to render this quad.
     * @param lakesTriangles The triangles rendering each lake, 
     * taken separately.
     */
    LakeRenderable(
        ShaderProgramPtr shaderProgram, 
        std::vector<
            std::pair<
                std::vector<int>, 
                std::vector<glm::vec3> 
            >
        >& lakesTriangles
    );

    /**
     * @brief Set the material of the renderable
     *
     * @param material The material
     */
    void setMaterial(const MaterialPtr& material);

private:
    
    /**
     * @brief 
     * Implementation of the private inherited function do_draw.
     *
     * @see Renderable.hpp
     */
    void do_draw();

    /**
     * @brief 
     * Implementation of the private inherited function do_animate.
     *
     * @see Renderable.hpp
     */
    void do_animate( float time );

    /**
     * @brief 
     * Vector dedicated to store the positions to send to 
     * the GPU buffer in order to render the map.
     */
    std::vector< glm::vec3 > m_positions;

    /**
     * @brief
     * IDs of the GPU's buffers associated with the positions
     * used for rendering.
     */
    GLuint m_pBuffer;

    /**
     * @brief The texture ID
     */
    GLuint m_textureId;

    /**
     * @brief
     * The material of the map
     */
    MaterialPtr m_material;
};

typedef std::shared_ptr<LakeRenderable> LakeRenderablePtr;

#endif
