/**
 * @file SeaRenderable.hpp
 *
 * @brief Simply a renderable for the sea
 * Can be use as a mip mapped plane
 */

#ifndef SEARENDERABLE_HPP
#define SEARENDERABLE_HPP




#include "../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "MapGenerator.hpp"

#include <list>
#include <utility>
#include <vector>
#include <glm/glm.hpp>

/**
 * @brief SeaRenderable
 */
class SeaRenderable : public HierarchicalRenderable
{
public :

    /**
     * @brief Destructor
     */
    ~SeaRenderable();

    /**
     * @brief Constructor
     *
     * @param shaderProgram Shader program used to render this quad.
     * @param mapParameters Parameters for the map generation
     * @param p1            First quad vertex.
     * @param p2            Second quad vertex.
     * @param p3            Third quad vertex.
     * @param p4            Fourth quad vertex.
     */
    SeaRenderable(ShaderProgramPtr shaderProgram, 
		  MapGenerator & mapGenerator,
		  const glm::vec3& p1,
		  const glm::vec3& p2,
		  const glm::vec3& p3,
		  const glm::vec3& p4);

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
     * Map generation parameters
     */
    MapParameters m_mapParameters;

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

typedef std::shared_ptr<SeaRenderable> SeaRenderablePtr;








#endif
