/**
 * @file MapRenderable.hpp
 *
 * @brief
 * This file contains the interface of the bi-dimensional map
 * renderable. In other words, the class MapRenderable will
 * be in charge of rendering the generated map, within the plane.
 */
#ifndef MAP_2D_RENDERABLE_HPP
#define MAP_2D_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "MapGenerator.hpp"

#include <list>
#include <utility>
#include <vector>
#include <glm/glm.hpp>

/**
 * @class MapRenderable
 *
 * @brief
 * The class MapRenderable implements the rendering step of the
 * bi-dimensional map.
 */
class MapRenderable : public HierarchicalRenderable
{
public:
    /**
     * @brief 
     * Construct the map renderable thanks to the data generated
     * by the map generator given as parameter.
     *
     * @param shaderProgram Shader program used to render the map.
     * @param mapGenerator Reference on the MapGenerator which generated
     * the map data.
     */
    MapRenderable(ShaderProgramPtr shaderProgram, MapGenerator& mapGenerator);

    /**
     * @brief
     * Default destructor, nothing particular to say about it.
     */
    ~MapRenderable();

    /**
     * @brief
     * Setter for the material of the map
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
    void do_animate(float time);

    /**
     * @brief 
     * Vector dedicated to store the positions to send to 
     * the GPU buffer in order to render the map.
     */
    std::vector<glm::vec3> m_positions;

    /**
     * @brief
     * Vector dedicated to store the texture coordinates
     * to access the height map
     */
    std::vector<glm::vec2> m_texCoords;

    /**
     * @brief
     * IDs of the GPU's buffers associated with the positions
     * used for rendering.
     */
    unsigned int m_positionsBufferID;

    /**
     * @brief
     * IDs of the GPU's buffers associated with the texture
     * used for rendering.
     */
    unsigned int m_heightmapID;

    /**
     * @brief
     * IDs of the texture coordinates
     */
    unsigned int m_texCoordsID;

    /**
     * @brief
     * The minimum altitude of the heightmap associated with the map.
     * It will be used when applying a scaling during the tessellation
     * step.
     */
    float m_minAltitude;

    /**
     * @brief
     * The scale altitude of the heightmap associated with the map.
     * It will be used when applying a scaling during the tessellation
     * step.
     */
    float m_scaleAltitude;

    /**
     * @brief
     * A reference on the MapGenerator used to generate the
     * map.
     */
    MapGenerator& m_mapGenerator;


    /**
     * @brief
     * The material of the map
     */
    MaterialPtr m_material;


    /**
     * @brief The texture ID for the biome 'Sea'
     */
    GLuint m_seaTexId;

    /**
     * @brief The texture ID for the biomse 'InnerBeach' and 'OuterBeach'
     */
    GLuint m_sandTexId;

    /**
     * @brief The texture ID for the biome 'Plains'
     */
    GLuint m_plainsTexId;

    /**
     * @brief The texture ID for the biome 'Lake'
     */
    GLuint m_lakeTexId;

    /**
     * @brief The texture ID for the biome 'Mountain'
     */
    GLuint m_mountainTexId;

    /**
     * @brief The texture ID for the biome 'Peak'
     */
    GLuint m_peakTexId;

    /**
     * @brief The texture ID for the first mask
     * (x, y, z, w) = (sea, sand, plains, lake)
     */
    GLuint m_seaSandPlainsLakeMaskId;

    /**
     * @brief The texture ID for the second mask
     * (x, y)        = (mountain, peak)
     */
    GLuint m_mountainPeakMaskId;

    /**
     * @brief Performs a sorted insert of a pair of coordinates into a 
     *  list according to its angle with the abscissa axis.
     *
     * @param list A reference of the list in which the insertion has to
     *  be performed.
     * @param x The abscissa of the pair to insert into the list.
     * @param y The ordinate of the pair to insert into the list.
     * @param centerX The abscissa of the center of the cell.
     * @param centerY The ordinate of the center of the cell.
     */
    void insertIntoList(
        std::list< std::pair<float, float> >& list,
        float x,
        float y,
        float centerX,
        float centerY
    );

    
    /**
     * @brief Cut the voronoi diagram into triangles
     * and send them
     */
    void sendVoronoiDiagram();

    /**
     * @brief Create and send the height map texture
     */
    void sendHeightMap();

    /**
     * @brief Compute the texture masks and send them
     */
    void sendMasks();

};

/**
 * @brief Typedef for smart pointer to map renderable.
 * */
typedef std::shared_ptr<MapRenderable> MapRenderablePtr; 

#endif
