/**
 * @file HeightBlob.hpp
 *
 * @brief Defining a height blob
 */

#ifndef HEIGHTBLOB_HPP
#define HEIGHTBLOB_HPP

#include "Seed.hpp"

class HeightBlob {

public :
    /**
     * @brief Consctructor
     * 
     * @param newPos    Position of the blob
     * @param newHeight Maximal height of the blob
     * @param newScale  Scale of the blob
     * @param newBiome  Biome of the blob
     */
    HeightBlob(Vertex2D newPos, float newHeight, 
	       float newScale,  Biome newBiome);

    /**
     * @brief Getter on the position
     *
     * @return The position of the blob
     */
    Vertex2D getPosition();

    /**
     * @brief Getter on the height
     *
     * @return The height of the blob
     */
    float getHeight();
    
    /**
     * @brief Getter on the scale
     *
     * @return The scale of the blob
     */
    float getScale();

    /**
     * @brief Getter on the biome
     *
     * @return The biome of the blob
     */
    Biome getBiome();


private :

    /// @brief Location of the blob
    Vertex2D position;
    /// @brief Maximal height of the blob
    float    height;
    /// @brief Scale of the blob
    float    scale;
    /// @brief Biome of the blob
    Biome    biome;

};





#endif
