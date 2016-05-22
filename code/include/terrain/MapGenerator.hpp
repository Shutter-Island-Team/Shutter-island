/**
 * @file MapGenerator.hpp
 *
 * @brief Main file of this directory 
 * Combine everything to generate a map
 */

#ifndef MAPGENERATOR_HPP
#define MAPGENERATOR_HPP

#include "VoronoiSeedsGenerator.hpp"

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

private :

    /// @brief Size of the map
    float mapSize;

    /// @brief The seed position generator
    VoronoiSeedsGenerator voronoiSeedsGenerator;

    /// @brief The seeds container
    voro::container seedsContainer;


};


#endif
