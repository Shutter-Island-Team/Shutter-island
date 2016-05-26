/**
 * @file HeightData.hpp
 *
 * @brief Class to store the height-related date of location
 */

#ifndef HEIGHTDATA_HPP
#define HEIGHTDATA_HPP

#include "Seed.hpp"

class HeightData {

public :
    /**
     * @brief Consctructor
     * 
     * @param newPos    Position
     * @param newHeight Height
     * @param newBiome  Biome
     */
    HeightData(Vertex2D newPos, float newHeight, Biome newBiome);

    /**
     * @brief Getter on the position
     *
     * @return The position
     */
    Vertex2D& getPosition();

    /**
     * @brief Getter on the height
     *
     * @return The height of the location
     */
    float getHeight();
    
    /**
     * @brief Getter on the biome
     *
     * @return The biome of the location
     */
    Biome getBiome();


private :

    /// @brief Location of the location
    Vertex2D position;
    /// @brief Height of the location
    float    height;
    /// @brief Biome of the location
    Biome    biome;

};





#endif
