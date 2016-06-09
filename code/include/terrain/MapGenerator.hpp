/**
 * @file MapGenerator.hpp
 *
 * @brief Main file of this directory 
 * Combine everything to generate a map
 */

#ifndef MAPGENERATOR_HPP
#define MAPGENERATOR_HPP

#include "VoronoiSeedsGenerator.hpp"
#include "HeightTree.hpp"
#include "MapParameters.hpp"
#include "../structures/Matrix.hpp"

#include <glm/glm.hpp>

/**
 * @brief 
 * Promise of definition of the friend class.
 */
class MapRenderable;

/**
 * @brief
 * The class MapGenerator encapsulates the generation of the seeds, the
 * computation of the associated Voronoi diagram and the computation of
 * the HeightMap.
 */
class MapGenerator {

    /**
     * @brief
     * Friend class definition, in order to access the MapGenerator's attributes
     * without using getters.
     */
    friend class MapRenderable;

    /**
     * @brief
     * Friend class definition, in order to access the MapGenerator's attributes
     * without using getters.
     */
    friend class SeaRenderable;

public :

    /**
     * @brief Constructor
     *
     * @param parameters A reference on the MapParameter object containing
     * the parsed parameters.
     * @param size Size of the map to create
     */
    MapGenerator(MapParameters& parameters, float size);

    /**
     * @brief Compute the map
     */
    void compute();

    /**
     * @brief Get the biome associated to a location
     * 
     * @param x Abscissa of the desired position
     * @param y Ordinna  of the desired position 
     *
     * @return The biome of the position
     */
    Biome getBiome(float x, float y);

    /**
     * @brief Get the approximative biome 
     * associated to a location using a sampled biome map
     * 
     * @param x Abscissa of the desired position
     * @param y Ordinna  of the desired position 
     *
     * @return A biome close to the position
     */
    Biome getApproximativeBiome(float x, float y);
    
    /**
     * @brief Get the centroid of a biome associated to a location
     * 
     * @param x Abscissa of the desired position
     * @param y Ordinna  of the desired position 
     *
     * @param xCentroid The abscissa of the centroid
     * @param yCentroid The ordinna of the centroid
     */
    void getCentroid(float x, float y, 
		     float& xCentroid, float& yCentroid);

    /**
     * @brief Get the height of a position
     *
     * @param x Abscissa of the desired position
     * @param y Ordinna  of the desired position 
     *
     * @return The biome of the position
     */
    float getHeight(float x, float y);

    /**
     * @brief Get the approximative height 
     * associated to a location using a sampled height map
     * 
     * @param x Abscissa of the desired position
     * @param y Ordinna  of the desired position 
     *
     * @return An interpolated height
     */
    float getApproximativeHeight(float x, float y);

    /**
     * @brief
     * Allows access to the list of the Lake biome through iterators.
     * 
     * @param begin A reference on the iterator in which the "begin iterator" on
     * the vector of Lake biomes should be stored.
     * @param end A reference on the iterator in which the "end iterator" on
     * the vector of Lake biomes should be stored.
     *
     * @return A boolean representing "Is there at least a lake on the map ?".
     */
    bool getLakes(
		  std::vector<glm::vec2>::iterator& begin, 
		  std::vector<glm::vec2>::iterator& end
		  );

    /**
     * @brief
     * Finds closest Lake biome.
     * Encapsulates "findClosestLake".
     * @see MapUtils.hpp
     *
     * @param x The abscissa of the point this function aims at dertermining the
     * closest Lake biome.
     * @param y The ordinate of the point this function aims at dertermining the
     * closest Lake biome.
     * @param xLake A reference on a float in which the function is going to store
     * the abscissa of the centroid of the closest Lake biome.
     * @param yLake A reference on a float in which the function is going to store
     * the ordinate of the centroid of the closest Lake biome.
     *
     * @return A boolean representing "Closest Lake biome found ?"
     */
    bool getClosestLake(
			float x,
			float y,
			float& xLake,
			float& yLake
			);

    /**
     * @brief Destructor
     */
    ~MapGenerator();

private :

    /**
     * @brief
     * A reference on the MapParameters object containing the parsed parameters.
     */
    MapParameters& m_mapParameters;

    /// @brief Size of the map
    float mapSize;

    /// @brief The seed position generator
    VoronoiSeedsGenerator voronoiSeedsGenerator;

    /// @brief The seeds vector
    std::vector<Seed> seeds;

    /// @brief The voronoi seeds container
    voro::container seedsContainer;

    /// @brief A 'biome map' use to accelerate the computation of a biome
    Biome *biomeMap = NULL;

    /// @brief The sampled height map
    float *heightMap = NULL;

    /**
     * @brief
     * A vector containing the coordinates of the centroids of the Lake biomes.
     */
    std::vector<glm::vec2> m_lakes;

    /// @brief The height tree
    HeightTree *heightTree = NULL;

    /**
     * @brief Clip a position to search inside the map
     *
     * @param x The abcissa of the position
     * @param y The ordinna of the position
     *
     * @return The clipped position
     */
    Vertex2D clipPosition(float x, float y);

};

#endif
