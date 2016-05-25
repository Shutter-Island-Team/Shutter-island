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

class MapGenerator {

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
     * @param position Desired position
     *
     * @return The biome of the position
     */
    Biome getBiome(float x, float y);

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
