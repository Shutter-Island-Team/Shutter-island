/**
 * @file Seed.hpp
 *
 * @brief Everything related to a seed of the map
 */
#ifndef SEED_HPP
#define SEED_HPP



#include "Biome.hpp"
#include "../../lib/voro++/src/voro++.hh"
#include <utility>
#include <memory>


typedef std::pair<float, float> Vertex2D;

typedef std::shared_ptr<voro::voronoicell_neighbor> voroNeighborPtr;

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
     * @brief Copy constructor
     * 
     * @param seed The Seed to copy in order to initialize the
     *  current Seed.
     */
    Seed(const Seed& seed);

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
    float getX() const;


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
    float getY() const;

    /**
     * @brief Setter on the abscissa of the centroid of the seed
     *
     * @param newX The new abscissa
     */
    void setCentroidX(float newX);

    /**
     * @brief Getter on the abscissa of the centroid of the seed
     *
     * @return The abscissa
     */
    float getCentroidX() const;

    /**
     * @brief Setter on the ordinna of the centroid of the seed
     *
     * @param newY The new ordinna
     */
    void setCentroidY(float newY);

    /**
     * @brief Getter on the ordinna of the centroid of the seed
     *
     * @return The ordinna
     */
    float getCentroidY() const;

    /**
     * @brief Setter on the biome of the seed
     *
     * @param newBiome The new biome
     */
    void setBiome(Biome newBiome);

    /**
     * @brief Getter on the Biome of the seed
     *
     * @return The biome
     */
    Biome getBiome() const;


    /**
     * @brief Setter for the cell of the seed
     * 
     * @param newCell The new cell
     */
    void setCell(voroNeighborPtr newCell);

    /**
     * @brief Getter on the cell of the seed
     *
     * @return A copy of the cell
     */
     voroNeighborPtr getCell() const;

    /**
     * @brief Overrides the assignment operator.
     *
     * @param seed The Seed to assign to the calling Seed.
     * @return The assigned seed.
     */
    Seed& operator = (const Seed& seed);

    /**
     * @brief Destroyer
     */
    ~Seed();

private:

    /// @brief The position of the seed.
    Vertex2D position;

    /// @brief The centroid associated with the seed.
    Vertex2D centroid;

    /// @brief The type of the biome of the seed.
    Biome biomeType;

    /// @brief The cell.
    voroNeighborPtr cell;
};

#endif
