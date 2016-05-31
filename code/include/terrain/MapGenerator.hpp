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

#include <glm/glm.hpp>

/**
 * @brief 
 * Promise of definition of the friend class.
 */
class Map2DRenderable;

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
friend class Map2DRenderable;

public :

    /**
     * @brief Constructor
     *
     * @param size Size of the map to create
     */
    MapGenerator(float size);

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

    /// @brief Size of the map
    float mapSize;

    /// @brief The seed position generator
    VoronoiSeedsGenerator voronoiSeedsGenerator;

    /// @brief The seeds vector
    std::vector<Seed> seeds;

    /// @brief The voronoi seeds container
    voro::container seedsContainer;

	/**
	 * @brief
	 * A vector containing the coordinates of the centroids of the Lake biomes.
	 */
	std::vector<glm::vec2> m_lakes;

    /// @brief The height tree
    HeightTree *heightTree = NULL;

};


#endif
