/**
 * @file Map2DRenderable.hpp
 *
 * @brief
 * This file contains the interface of the bi-dimensional map
 * renderable. In other words, the class Map2DRenderable will
 * be in charge of rendering the generated map, within the plane.
 */
#ifndef MAP_2D_RENDERABLE_HPP
#define MAP_2D_RENDERABLE_HPP

#include "../Renderable.hpp"
#include "MapGenerator.hpp"

#include <list>
#include <utility>
#include <vector>
#include <glm/glm.hpp>

class Map2DRenderable : public Renderable
{
public:
    /**
     * @brief 
     * Construct the map renderable thanks to the data generated
     * by the map generator given as parameter.
     *
     * @param program Shader program used to render the map.
     * @param mapGenerator Reference on the MapGenerator which generated
     * the map data.
     */
    Map2DRenderable(ShaderProgramPtr shaderProgram, MapGenerator& mapGenerator);

    /**
     * @brief
     * Default destructor, nothing particular to say about it.
     */
    ~Map2DRenderable();
private:
    /*
     * Inherited functions.
     * Their specifications are in the Renderable class header.
     */
    void do_draw();
    void do_animate(float time);

    /**
     * @brief 
     * Vectors dedicated to store the positions to send to 
     * the GPU buffer in order to render the map.
     */
    std::vector<glm::vec3> m_positions;

    /**
     * @brief 
     * Vectors dedicated to store the colors to send to 
     * the GPU buffer in order to render the map.
     */
    std::vector<glm::vec4> m_colors;

    /**
     * @brief 
     * Vectors dedicated to store the normals to send to 
     * the GPU buffer in order to render the map.
     */
    std::vector<glm::vec3> m_normals;

    /**
     * @brief
     * IDs of the GPU's buffers associated with the positions
     * used for rendering.
     */
    unsigned int m_positionsBufferID;

    /**
     * @brief
     * IDs of the GPU's buffers associated with the colors
     * used for rendering.
     */
    unsigned int m_colorsBufferID;

    /**
     * @brief
     * IDs of the GPU's buffers associated with the normals
     * used for rendering.
     */
    unsigned int m_normalsBufferID;

    /**
     * @brief
     * A reference on the MapGenerator used to generate the
     * map.
     */
    MapGenerator& m_mapGenerator;

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
     * @brief Returns the color corresponding to a biome.
     * 
     * @param biome The biome to get the color.
     * @return The color corresponding to the biome.
     */
    glm::vec4 biomeColor(Biome biome);
};

/**
 * @brief Typedef for smart pointer to map renderable.
 * */
typedef std::shared_ptr<Map2DRenderable> Map2DRenderablePtr; 

#endif
