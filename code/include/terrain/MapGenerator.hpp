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
     * @brief Get the height of a position
     *
     * @param x Abscissa of the desired position
     * @param y Ordinna  of the desired position 
     *
     * @return The biome of the position
     */
    float getHeight(float x, float y);

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

    /// @brief The height tree
    HeightTree *heightTree = NULL;

};


#endif
