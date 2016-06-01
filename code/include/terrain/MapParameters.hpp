/**
 * @file MapParameters.hpp
 *
 * @brief
 * This file contains the header file associated with the MapParameters class.
 */
#ifndef MAP_PARAMETERS_HPP
#define MAP_PARAMETERS_HPP

#include <string>

/**
 * @class MapParameters
 *
 * @brief
 * Encapsulates all the "defines parameters" associated with the map
 * generation, so as to parse them through RapidJSon.
 * Thanks to this, we can modify the parameters without compiling.
 */
class MapParameters
{
public:
    /**
     * @brief Constructor for the MapParameters class from a filename,
     * representing the JSon file where the parameters are stored.
     *
     * @param filename The name of the JSon file to parse.
     */
    MapParameters(const std::string& filename);

    /**************************************************************************
     * Biome class "getters".
     *************************************************************************/
    /**
     * @brief
     * Getter on m_heightMinSea.
     *
     * @return The value of m_heightMinSea.
     */
    float getHeightMinSea();

    /**
     * @brief
     * Getter on m_heightMaxSea.
     *
     * @return The value of m_heightMaxSea.
     */
    float getHeightMaxSea();

    /**
     * @brief
     * Getter on m_heightMinLake.
     *
     * @return The value of m_heightMinLake.
     */
    float getHeightMinLake();

    /**
     * @brief
     * Getter on m_heightMaxLake.
     *
     * @return The value of m_heightMaxLake.
     */
    float getHeightMaxLake();


    /**
     * @brief
     * Getter on m_heightMinInnerBeach.
     *
     * @return The value of m_heightMinInnerBeach.
     */
    float getHeightMinInnerBeach();

    /**
     * @brief
     * Getter on m_heightMaxInnerBeach.
     *
     * @return The value of m_heightMaxInnerBeach.
     */
    float getHeightMaxInnerBeach();

    /**
     * @brief
     * Getter on m_heightMinOutterBeach.
     *
     * @return The value of m_heightMinOutterBeach.
     */
    float getHeightMinOutterBeach();

    /**
     * @brief
     * Getter on m_heightMaxOutterBeach.
     *
     * @return The value of m_heightMaxOutterBeach.
     */
    float getHeightMaxOutterBeach();


    /**
     * @brief
     * Getter on m_heightMinPlains.
     *
     * @return The value of m_heightMinPlains.
     */
    float getHeightMinPlains();

    /**
     * @brief
     * Getter on m_heightMaxPlains.
     *
     * @return The value of m_heightMaxPlains.
     */
    float getHeightMaxPlains();

    /**
     * @brief
     * Getter on m_heightMinMountain.
     *
     * @return The value of m_heightMinMountain.
     */
    float getHeightMinMountain();

    /**
     * @brief
     * Getter on m_heightMaxMountain.
     *
     * @return The value of m_heightMaxMountain.
     */
    float getHeightMaxMountain();

    /**
     * @brief
     * Getter on m_heightMinPeak.
     *
     * @return The value of m_heightMinPeak.
     */
    float getHeightMinPeak();

    /**
     * @brief
     * Getter on m_heightMaxPeak.
     *
     * @return The value of m_heightMaxPeak.
     */
    float getHeightMaxPeak();

    /**
     * @brief
     * Getter on m_scaleLimitInfluence.
     *
     * @return The value of m_scaleLimitInfluence.
     */
    float getScaleLimitInfluence();

    /**
     * @brief
     * Getter on m_biomeDepthMin.
     *
     * @return The value of m_biomeDepthMin.
     */
    int getBiomeDepthMin();

    /**
     * @brief
     * Getter on m_biomeDepthMax.
     *
     * @return The value of m_biomeDepthMax.
     */
    int getBiomeDepthMax();
    /**************************************************************************
     * End of Biome class "getters".
     *************************************************************************/

    /**************************************************************************
     * BiomeRepartition class "getters".
     *************************************************************************/
    /**
     * @brief
     * Getter on m_landBlendingCoefficient.
     *
     * @return The value of m_landBlendingCoefficient.
     */
    float getLandBlendingCoefficient();

    /**
     * @brief
     * Getter on m_lakeGeometricPicking.
     * 
     * @return The value of m_lakeGeometricPicking.
     */
    float getLakeGeometricPicking();

    /**
     * @brief
     * Getter on m_lakeProbTransform.
     * 
     * @return The value of m_lakeProbTransform.
     */
    float getLakeProbTransform();

    /**
     * @brief
     * Getter on m_lakePositiveInfluence.
     * 
     * @return The value of m_lakePositiveInfluence.
     */
    float getLakePositiveInfluence();

    /**
     * @brief
     * Getter on m_mountainGeometricPicking.
     * 
     * @return The value of m_mountainGeometricPicking.
     */
    float getMountainGeometricPicking();

    /**
     * @brief
     * Getter on m_mountainProbTransform.
     * 
     * @return The value of m_mountainProbTransform.
     */
    float getMountainProbTransform();

    /**
     * @brief
     * Getter on m_mountainMaxTry.
     * 
     * @return The value of m_mountainMaxTry.
     */
    int getMountainMaxTry();
    /**************************************************************************
     * End of BiomeRepartition class "getters".
     *************************************************************************/

    /**************************************************************************
     * MapGenerator class "getters".
     *************************************************************************/
    /**
     * @brief
     * Getter on m_nbSeeds.
     *
     * @return The value of m_nbSeeds.
     */
    int getNbSeeds(); 

    /**
     * @brief
     * Getter on m_nbSubdivision.
     *
     * @return The value of m_nbSubdivision.
     */
    int getNbSubdivision();

    /**
     * @brief
     * Getter on m_nbSeedsMaxSubdiv.
     *
     * @return The value of m_nbSeedsMaxSubdiv.
     */
    int getNbSeedsMaxSubdiv();

    /**
     * @brief
     * Getter on m_distMin.
     *
     * @return The value of m_distMin.
     */
    int getDistMin();
    /**************************************************************************
     * End ofMapGenerator class "getters".
     *************************************************************************/

private:
    /**************************************************************************
     * Biome class "defines".
     *************************************************************************/
    /**
     * @brief
     * Defines the minimal height associated with a biome of sea.
     */
    float m_heightMinSea;

    /**
     * @brief
     * Defines the maximal height associated with a biome of sea.
     */
    float m_heightMaxSea;

    /**
     * @brief
     * Defines the minimal height associated with a biome of lake.
     * The value is more important than the sea to make sure it carves the land.
     */
    float m_heightMinLake;

    /**
     * @brief
     * Defines the maximal height associated with a biome of lake.
     * The value is more important than the sea to make sure it carves the land.
     */
    float m_heightMaxLake;


    /**
     * @brief
     * Defines the minimal height associated with a biome "InnerBeach".
     */
    float m_heightMinInnerBeach;

    /**
     * @brief
     * Defines the maximal height associated with a biome "InnerBeach".
     */
    float m_heightMaxInnerBeach;

    /**
     * @brief
     * Defines the minimal height associated with a biome "OutterBeach".
     */
    float m_heightMinOutterBeach;

    /**
     * @brief
     * Defines the maximal height associated with a biome "OutterBeach".
     */
    float m_heightMaxOutterBeach;

    /**
     * @brief
     * Defines the minimal height associated with a biome "Plains".
     */
    float m_heightMinPlains;

    /**
     * @brief
     * Defines the maximal height associated with a biome "Plains".
     */
    float m_heightMaxPlains;

    /**
     * @brief
     * Defines the minimal height associated with a biome "Mountain".
     * (actually a hill)
     */
    float m_heightMinMountain;

    /**
     * @brief
     * Defines the maximal height associated with a biome "Mountain".
     * (actually a hill)
     */
    float m_heightMaxMountain;

    /**
     * @brief
     * Defines the minimal height associated with a biome "Peak".
     */
    float m_heightMinPeak;

    /**
     * @brief
     * Defines the maximal height associated with a biome "Peak".
     */
    float m_heightMaxPeak;

    /**
     * @brief
     * A scale parameter to limit the inluence of a biome on other biomes.
     */
    float m_scaleLimitInfluence;

    /**
     * @brief
     * Defines the minimal number of recursive subdivisions in the QuadTree used to
     * generate the HeightMap.
     */
    int m_biomeDepthMin;

    /**
     * @brief
     * Defines the maximal number of recursive subdivisions in the QuadTree used to
     * generate the HeightMap.
     */
    int m_biomeDepthMax;
    /**************************************************************************
     * End of Biome class "defines".
     *************************************************************************/

    /**************************************************************************
     * BiomeRepartition class "defines".
     *************************************************************************/
    /**
     * @brief
     * When deciding if a biome is going to be a land biome, we take into account 
     * both its distance to the center of the map and the proportion of its
     * neighbours being land biomes.
     * Defines the coefficient used to ponderate the importance of the former in
     * comparison with the latter.
     */
    float m_landBlendingCoefficient;

    /**
     * @brief
     * Defines the parameter of the geometric law used to generate Lake biomes.
     */
    float m_lakeGeometricPicking;

    /**
     * @brief
     * Defines the probability that a Lake-eligible biome is effectively
     * transformed into a Lake biome.
     */
    float m_lakeProbTransform;

    /**
     * @brief
     * When a biome is transformed into a Lake, it has a special influence on its
     * neighbours. As a matter of fact, it fosters the fact that its Lake-eligible
     * neighbours are transformed into Lake too.
     * Concretely, it defines the "bonus" probability added to the initial one when
     * the previously described situation occurs.
     */
    float m_lakePositiveInfluence;

    /**
     * @brief
     * Defines the parameter of the geometric law used to generate Mountain biomes.
     */
    float m_mountainGeometricPicking;

    /**
     * @brief
     * Defines the probability that a Mountain-eligible biome is effectively
     * transformed into a Mountain biome.
     */
    float m_mountainProbTransform;

    /**
     * @brief
     * Defines the maximum number of iterations of the geometric law used
     * to generate Mountain biomes.
     * If this maximum is reached, then we stop trying to generate mountains.
     */
    int m_mountainMaxTry;
    /**************************************************************************
     * End of BiomeRepartition class "defines".
     *************************************************************************/

    /**************************************************************************
     * MapGenerator class "defines".
     *************************************************************************/
    /**
     * @brief
     * Defines the default number of seeds to be generated by the
     * VoronoiSeedsGenerator.
     */
    int m_nbSeeds = 300; 

    /**
     * @brief
     * Defines the default number of subdivisions for each dimension 
     * of the grid used within the VoronoiSeedsGenerator.
     */
    int m_nbSubdivision = 25;

    /**
     * @brief
     * Defines the default number of seeds by subdivisions 
     * of the grid used within the VoronoiSeedsGenerator.
     */
    int m_nbSeedsMaxSubdiv = 5;

    /**
     * @brief
     * Defines the default minimal distance between two generated seeds.
     */
    int m_distMin = 10;
    /**************************************************************************
     * End of MapGenerator class "defines".
     *************************************************************************/
};

#endif
