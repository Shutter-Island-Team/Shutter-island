/**
 * @file Seed.hpp
 *
 * @brief Everything related to a seed of the map
 */
#ifndef SEED_HPP
#define SEED_HPP


#include "Biome.hpp"
#include <utility>


typedef std::pair<float, float> Vertex2D;


class Seed {

public:
    /**
     * @brief Constructor
     * 
     * @param x     Abscissa of the seed
     * @param y     Ordinna of the seed
     * @param biome Type of the biome (default : Undefined)
     */
    Seed(float x, float y, Biome biome = Undefined);



    /**
     * @brief Setter on the abscissa of the seed
     *
     * @param newX The new abscissa
     */
    void setX(float newX);

    /**
     * @brief Getter on the abscissa of the seed
     *
     * @return The abscissa
     */
    float getX();


    /**
     * @brief Setter on the ordinna of the seed
     *
     * @param newY The new ordinna
     */
    void setY(float newY);

    /**
     * @brief Getter on the ordinna of the seed
     *
     * @return The ordinna
     */
    float getY();



    /**
     * @brief Setter on the biome of the seed
     *
     * @param newX The new biome
     */
    void setBiome(Biome newBiome);

    /**
     * @brief Getter on the Biome of the seed
     *
     * @return The biome
     */
    Biome getBiome();


private:

    /// @brief The position of the seed
    Vertex2D position;

    /// @brief The type of the biome of the seed
    Biome biomeType;
};

#endif
