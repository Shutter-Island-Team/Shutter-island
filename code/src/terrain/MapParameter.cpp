#include <cstring>
#include <fstream>
#include <iostream>
#include <streambuf>

#include "../../include/rapidjson/document.h"
#include "../../include/terrain/MapParameters.hpp"

MapParameters::MapParameters(const std::string& filename)
{
    /*
     * Creating a rapidJSon document so as to parse the JSon configuration
     * file.
     */
    rapidjson::Document document; 
    std::ifstream file(filename);
    std::string fileContent;

    /*
     * Resizing the "fileContent" buffer to the size of the content.
     */
    file.seekg(0, std::ios::end);
    fileContent.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    /*
     * Copying the content of the file into the "fileContent" buffer.
     */
    fileContent.assign(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );

    /*
     * Parsing the JSon file, and assigning the parsed value to the
     * stored attributes.
     */
    document.Parse(fileContent.c_str());

    /*
     * Biome "defines".
     */
    m_heightMinSea = document["Biome"]["heightMinSea"].GetFloat();
    m_heightMaxSea = document["Biome"]["heightMaxSea"].GetFloat();
    m_heightMinLake = document["Biome"]["heightMinLake"].GetFloat();
    m_heightMaxLake = document["Biome"]["heightMaxLake"].GetFloat();
    m_heightMinInnerBeach = document["Biome"]["heightMinInnerBeach"].GetFloat();
    m_heightMaxInnerBeach = document["Biome"]["heightMaxInnerBeach"].GetFloat();
    m_heightMinOuterBeach = document["Biome"]["heightMinOuterBeach"].GetFloat();
    m_heightMaxOuterBeach = document["Biome"]["heightMaxOuterBeach"].GetFloat();
    m_heightMinPlains = document["Biome"]["heightMinPlains"].GetFloat();
    m_heightMaxPlains = document["Biome"]["heightMaxPlains"].GetFloat();
    m_heightMinMountain = document["Biome"]["heightMinMountain"].GetFloat();
    m_heightMaxMountain = document["Biome"]["heightMaxMountain"].GetFloat();
    m_heightMinPeak = document["Biome"]["heightMinPeak"].GetFloat();
    m_heightMaxPeak = document["Biome"]["heightMaxPeak"].GetFloat();
    m_scaleLimitInfluence = document["Biome"]["scaleLimitInfluence"].GetFloat();
    m_biomeDepthMin = document["Biome"]["biomeDepthMin"].GetInt();
    m_biomeDepthMax = document["Biome"]["biomeDepthMax"].GetInt();

    /*
     * Biome Repartition "defines".
     */
     m_landBlendingCoefficient = document["BiomeRepartition"]["landBlendingCoefficient"].GetFloat();
     m_lakeGeometricPicking = document["BiomeRepartition"]["lakeGeometricPicking"].GetFloat();
     m_lakeProbTransform = document["BiomeRepartition"]["lakeProbTransform"].GetFloat();
     m_lakePositiveInfluence = document["BiomeRepartition"]["lakePositiveInfluence"].GetFloat();
     m_mountainGeometricPicking = document["BiomeRepartition"]["mountainGeometricPicking"].GetFloat();
     m_mountainProbTransform = document["BiomeRepartition"]["mountainProbTransform"].GetFloat();
     m_mountainMaxTry = document["BiomeRepartition"]["mountainMaxTry"].GetInt();

     /*
      * MapGenerator "defines".
      */
     m_nbSeeds = document["MapGenerator"]["nbSeeds"].GetInt();
     m_nbSubdivision = document["MapGenerator"]["nbSubdivision"].GetInt();
     m_nbSeedsMaxSubdiv = document["MapGenerator"]["nbSeedsMaxSubdiv"].GetInt();
     m_distMin = document["MapGenerator"]["distMin"].GetInt();

     /*
      * HeightTree "defines".
      */
     m_detectionThreshold = document["HeightTree"]["detectionThreshold"].GetFloat();
     m_heightBlendingCoefficient = document["HeightTree"]["heightBlendingCoefficient"].GetFloat();

     /*
      * VoronoiSeedsGenerator "defines".
      */
     m_vsgWidth = document["VSG"]["vsgWidth"].GetFloat();
     m_vsgHeight = document["VSG"]["vsgHeight"].GetFloat();
     m_vsgNbSeeds = document["VSG"]["vsgNbSeeds"].GetInt();
     m_vsgWidthSub = document["VSG"]["vsgWidthSub"].GetInt();
     m_vsgHeightSub = document["VSG"]["vsgHeightSub"].GetInt();
     m_vsgMaxSeedsBySub = document["VSG"]["vsgMaxSeedsBySub"].GetInt();
     m_vsgMinDist = document["VSG"]["vsgMinDist"].GetFloat();
}

/*
 * Biome "getters".
 */
float MapParameters::getHeightMinSea()
{
    return m_heightMinSea;
}

float MapParameters::getHeightMaxSea()
{
    return m_heightMaxSea;
}

float MapParameters::getHeightMinLake()
{
    return m_heightMinLake;
}

float MapParameters::getHeightMaxLake()
{
    return m_heightMaxLake;
}


float MapParameters::getHeightMinInnerBeach()
{
    return m_heightMinInnerBeach;
}

float MapParameters::getHeightMaxInnerBeach()
{
    return m_heightMaxInnerBeach;
}

float MapParameters::getHeightMinOuterBeach()
{
    return m_heightMinOuterBeach;
}

float MapParameters::getHeightMaxOuterBeach()
{
    return m_heightMaxOuterBeach;
}

float MapParameters::getHeightMinPlains()
{
    return m_heightMinPlains;
}

float MapParameters::getHeightMaxPlains()
{
    return m_heightMaxPlains;
}

float MapParameters::getHeightMinMountain()
{
    return m_heightMinMountain;
}

float MapParameters::getHeightMaxMountain()
{
    return m_heightMaxMountain;
}

float MapParameters::getHeightMinPeak()
{
    return m_heightMinPeak;
}

float MapParameters::getHeightMaxPeak()
{
    return m_heightMaxPeak;
}

float MapParameters::getScaleLimitInfluence()
{
    return m_scaleLimitInfluence;
}

int MapParameters::getBiomeDepthMin()
{
    return m_biomeDepthMin;
}

int MapParameters::getBiomeDepthMax()
{
    return m_biomeDepthMax;
}

/*
 * BiomeRepartition "getters".
 */
float MapParameters::getLandBlendingCoefficient()
{
    return m_landBlendingCoefficient;
}

float MapParameters::getLakeGeometricPicking()
{
    return m_lakeGeometricPicking;
}

float MapParameters::getLakeProbTransform()
{
    return m_lakeProbTransform;
}

float MapParameters::getLakePositiveInfluence()
{
    return m_lakePositiveInfluence;
}

float MapParameters::getMountainGeometricPicking()
{
    return m_mountainGeometricPicking;
}

float MapParameters::getMountainProbTransform()
{
    return m_mountainProbTransform;
}

int MapParameters::getMountainMaxTry()
{
    return m_mountainMaxTry;
}

/*
 * MapGenerator "getters".
 */
int MapParameters::getNbSeeds() 
{
    return m_nbSeeds;
}

int MapParameters::getNbSubdivision()
{
    return m_nbSubdivision;
}

int MapParameters::getNbSeedsMaxSubdiv()
{
    return m_nbSeedsMaxSubdiv;
}

int MapParameters::getDistMin()
{
    return m_distMin;
}

/*
 * HeightTree "getters".
 */
float MapParameters::getDetectionThreshold()
{
    return m_detectionThreshold;
}

float MapParameters::getHeightBlendingCoefficient()
{
    return m_heightBlendingCoefficient;
}

/*
 * VSG "getters".
 */
float MapParameters::getVsgWidth()
{
    return m_vsgWidth;
}

float MapParameters::getVsgHeight()
{
    return m_vsgHeight;
}

int MapParameters::getVsgNbSeeds()
{
    return m_vsgNbSeeds;
}

int MapParameters::getVsgWidthSub()
{
    return m_vsgWidthSub;
}

int MapParameters::getVsgHeightSub()
{
    return m_vsgHeightSub;
}

int MapParameters::getVsgMaxSeedsBySub()
{
    return m_vsgMaxSeedsBySub;
}

float MapParameters::getVsgMinDist()
{
    return m_vsgMinDist;
}
