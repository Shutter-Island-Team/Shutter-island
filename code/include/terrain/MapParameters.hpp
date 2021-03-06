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
     * Getter on m_heightMinOuterBeach.
     *
     * @return The value of m_heightMinOuterBeach.
     */
    float getHeightMinOuterBeach();

    /**
     * @brief
     * Getter on m_heightMaxOuterBeach.
     *
     * @return The value of m_heightMaxOuterBeach.
     */
    float getHeightMaxOuterBeach();


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

    /**************************************************************************
     * HeightTree class "getters".
     *************************************************************************/
    /**
     * @brief
     * Getter on m_detectionThreshold.
     *
     * @return The value of m_detectionThreshold.
     */
    float getDetectionThreshold();

    /**
     * @brief
     * Getter on m_heightBlendingCoefficient.
     *
     * @return The value of m_heightBlendingCoefficient.
     */
    float getHeightBlendingCoefficient();
    /**************************************************************************
     * End of HeightTree class "getters".
     *************************************************************************/

    /**************************************************************************
     * VSG class "getters".
     *************************************************************************/
    /**
     * @brief
     * Getter on m_vsgWidth.
     *
     * @return The value of m_vsgWidth.
     */
    float getVsgWidth();

    /**
     * @brief
     * Getter on m_vsgHeight.
     *
     * @return The value of m_vsgHeight.
     */
    float getVsgHeight();

    /**
     * @brief
     * Getter on m_vsgNbSeeds.
     *
     * @return The value of m_vsgNbSeeds.
     */
    int getVsgNbSeeds();

    /**
     * @brief
     * Getter on m_vsgWidthSub.
     *
     * @return The value of m_vsgWidthSub.
     */
    int getVsgWidthSub();

    /**
     * @brief
     * Getter on m_vsgHeightSub.
     *
     * @return The value of m_vsgHeightSub.
     */
    int getVsgHeightSub();

    /**
     * @brief
     * Getter on m_vsgMaxSeedsBySub.
     *
     * @return The value of m_vsgMaxSeedsBySub.
     */
    int getVsgMaxSeedsBySub();

    /**
     * @brief
     * Getter on m_vsgMinDist.
     *
     * @return The value of m_vsgMinDist.
     */
    float getVsgMinDist();
    /**************************************************************************
     * End of VSG class "getters".
     *************************************************************************/

    /**************************************************************************
     * MapRenderable class "getters".
     *************************************************************************/
    /**
     * @brief
     * Getter on m_tessellationLevel.
     *
     * @return The value of m_tessellationLevel.
     */
    float getTessellationLevel();

    /**
     * @brief
     * Getter on m_heightmapScaling.
     *
     * @return The value of m_heightmapScaling.
     */
    float getHeightmapScaling();

    /**
     * @brief
     * Getter on m_landTextureScaling.
     *
     * @return The value of m_landTextureScaling.
     */
    float getLandTextureScaling();

    /**
     * @brief
     * Getter on m_seaTextureScaling.
     *
     * @return The value of m_seaTextureScaling.
     */
    float getSeaTextureScaling();

    /**
     * @brief
     * Getter on m_lakeTextureScaling.
     *
     * @return The value of m_lakeTextureScaling.
     */
    float getLakeTextureScaling();

    /**************************************************************************
     * End of MapRenderable class "getters".
     *************************************************************************/
	
	 /**************************************************************************
	 * LakeRenderable class "getters".
	 *************************************************************************/
	 /**
	 * @brief
	 * Getter on m_lakesExtension.
	 *
	 * @return The value of m_lakesExtension.
	 */
	float getLakesExtension();
	/**************************************************************************
	* End of LakeRenderable class "getters".
	*************************************************************************/

    /**************************************************************************
     * Texture class "getters".
     *************************************************************************/
    /**
     * @brief
     * Getter on m_seaTextureExtent
     *
     * @return The value of m_seaTextureExtent
     */
    float getSeaTextureExtent();

    /**
     * @brief
     * Getter on m_sandTextureExtent
     *
     * @return The value of m_sandTextureExtent
     */
    float getSandTextureExtent();

    /**
     * @brief
     * Getter on m_plainsTextureExtent
     *
     * @return The value of m_plainsTextureExtent
     */
    float getPlainsTextureExtent();

    /**
     * @brief
     * Getter on m_lakeTextureExtent
     *
     * @return The value of m_lakeTextureExtent
     */
    float getLakeTextureExtent();

    /**
     * @brief
     * Getter on m_mountainTextureExtent
     *
     * @return The value of m_mountainTextureExtent
     */
    float getMountainTextureExtent();

    /**
     * @brief
     * Getter on m_peakTextureExtent
     *
     * @return The value of m_peakTextureExtent
     */
    float getPeakTextureExtent();
    /**************************************************************************
     * End of Texture "getters".
     *************************************************************************/

    /**************************************************************************
     * Global "getters".
     *************************************************************************/
    /**
     * @brief
     * Getter on m_mapSize.
     *
     * @return The value of m_mapSize.
     */
    float getMapSize();

	/**
	 * @brief
	 * Getter on m_importingHeightmap.
	 *
	 * @return The value of m_importingHeightmap.
	 */
	bool getImportingHeightmap();

	/**
	 * @brief
	 * Getter on m_importingSeeds.
	 *
	 * @return The value of m_importingSeeds.
	 */
	bool getImportingSeeds();

	/**
	 * @brief
	 * Getter on m_importSeeds.
	 *
	 * @return The value of m_importSeeds.
	 */
	std::string getImportSeeds();

	/**
	 * @brief
	 * Getter on m_importHeightmap.
	 *
	 * @return The value of m_importHeightmap.
	 */
	std::string getImportHeightmap();

	/**
	 * @brief
	 * Getter on m_exportMapEnabled.
	 *
	 * @return The value of m_exportMapEnabled.
	 */
	bool getExportMapEnabled();

	/**
	 * @brief
	 * Getter on m_exportSeeds.
	 *
	 * @return The value of m_exportSeeds.
	 */
	std::string getExportSeeds();

	/**
	 * @brief
	 * Getter on m_exportHeightmap.
	 *
	 * @return The value of m_exportHeightmap.
	 */
	std::string getExportHeightmap();

	/**
	 * @brief
	 * Getter on m_boidsEnabled.
	 *
	 * @return The value of m_boidsEnabled.
	 */
	bool getBoidsEnabled();

    /**************************************************************************
     * End of global "getters".
     *************************************************************************/
	
	/**************************************************************************
	 * MapParser class "getters".
	 *************************************************************************/
	/**
	 * @brief
	 * Getter on m_parserBufferSize.
	 *
	 * @return The value of m_parserBufferSize.
	 */
	int getParserBufferSize();
	/**************************************************************************
	 * End of MapParser class "getters".
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
     * Defines the minimal height associated with a biome "OuterBeach".
     */
    float m_heightMinOuterBeach;

    /**
     * @brief
     * Defines the maximal height associated with a biome "OuterBeach".
     */
    float m_heightMaxOuterBeach;

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
    int m_nbSeeds; 

    /**
     * @brief
     * Defines the default number of subdivisions for each dimension 
     * of the grid used within the VoronoiSeedsGenerator.
     */
    int m_nbSubdivision;

    /**
     * @brief
     * Defines the default number of seeds by subdivisions 
     * of the grid used within the VoronoiSeedsGenerator.
     */
    int m_nbSeedsMaxSubdiv;

    /**
     * @brief
     * Defines the default minimal distance between two generated seeds.
     */
    int m_distMin;
    /**************************************************************************
     * End of MapGenerator class "defines".
     *************************************************************************/

    /**************************************************************************
     * HeightTree class "defines".
     *************************************************************************/
    /**
     * @brief
     * Determining if a point is already in the tree before inserting it,
     * we have to compare the position of the point to insert with the positions
     * of all the already inserted points.
     * However, when comparing floats, such as the coordinates of the points, the
     * computation error has to be taken into account (so as to avoid false
     * negative).
     * This threshold aims precisely at that : if the distance between two points
     * is lesser than the former, the two points are considered equal.
     */
    float m_detectionThreshold;

    /**
     * @brief
     * When determining the height associated with a point, we iterate recursively
     * from the root of the tree to the corresponding node of the deepest level
     * of subdivision.
     * Each level contributes to the global height, but their contribution is
     * recursively reduced by applying the defined blending coefficient.
     */
    float m_heightBlendingCoefficient;
    /**************************************************************************
     * End of HeightTree class "defines".
     *************************************************************************/

    /**************************************************************************
     * VSG class "defines".
     *************************************************************************/
    /**
     * @brief
     * Default width of the constrained plane in which the seeds are generated.
     */
    float m_vsgWidth;

    /**
     * @brief
     * Default height of the constrained plane in which the seeds are generated.
     */
    float m_vsgHeight;

    /**
     * @brief
     * Default number of seeds to generate.
     */
    int m_vsgNbSeeds;

    /**
     * @brief
     * Default width of a subdivision of the grid used to tessellate the
     * constrained plane the seeds generation takes place within.
     */
    int m_vsgWidthSub;

    /**
     * @brief
     * Default height of a subdivision of the grid used to tessellate the
     * constrained plane the seeds generation takes place within.
     */
    int m_vsgHeightSub;

    /**
     * @brief
     * Default maximum of the number of seeds by subdivision of the grid
     * used to tessellate the constrained plane the seeds generation takes
     * place within.
     */
    int m_vsgMaxSeedsBySub;

    /**
     * @brief
     * Default minimum distance between two generated seeds.
     */
    float m_vsgMinDist;
    /**************************************************************************
     * End of VSG class "defines".
     *************************************************************************/

    /**************************************************************************
     * MapRenderable class "defines".
     *************************************************************************/
    /**
     * @brief
     * Multiplicative factor used in the Tessellation shaders so as to set the
     * tessellation level for both the outer and inner tessellations.
     */
    float m_tessellationLevel;

    /**
     * @brief
     * Scaling factor used to determine the precision (ie. the resolution) of
     * the texture storing the heightmap.
     * In other word, the resolution of the latter is equal to
     * "mapSize*heightmapScaling".
     */
    float m_heightmapScaling;

    /**
     * @brief
     * Defines the scaling for the texture application for the land
     */
    float m_landTextureScaling;

    /**
     * @brief
     * Defines the scaling for the texture application for the sea
     */
    float m_seaTextureScaling;

    /**
     * @brief
     * Defines the scaling for the texture application for the lakes
     */
    float m_lakeTextureScaling;
    /**************************************************************************
     * End of MapRenderable class "defines".
     *************************************************************************/

	 /**************************************************************************
	 * LakeRenderable class "defines".
	 *************************************************************************/
	 /**
	 * @brief
	 * Defines the extension coefficient applied to the lakes renderables.
	 */
	float m_lakesExtension;
	/**************************************************************************
	 * End of LakeRenderable class "defines".
	 *************************************************************************/

     /**************************************************************************
      * Texture "defines".
      *************************************************************************/
    /**
     * @brief
     * Defines the extent of a sea texture on its neighbours
     */
    float m_seaTextureExtent;

    /**
     * @brief
     * Defines the extent of a sand texture on its neighbours
     */
    float m_sandTextureExtent;

    /**
     * @brief
     * Defines the extent of a plains texture on its neighbours
     */
    float m_plainsTextureExtent;

    /**
     * @brief
     * Defines the extent of a lake texture on its neighbours
     */
    float m_lakeTextureExtent;

    /**
     * @brief
     * Defines the extent of a mountain texture on its neighbours
     */
    float m_mountainTextureExtent;

    /**
     * @brief
     * Defines the extent of a peak texture on its neighbours
     */
    float m_peakTextureExtent;
    /**************************************************************************
     * End of texture "defines".
     *************************************************************************/

    /**************************************************************************
     * Global "defines".
     *************************************************************************/
    /**
     * @brief
     * Defines the size of the square map.
     */
    float m_mapSize;

	/**
	* @brief
	* Defines whether the seeds' data are import from external files or not.
	*/
	bool m_importingSeeds;

	/**
	 * @brief
	 * Defines whether the heightmap data are import from external files or not.
	 */
	bool m_importingHeightmap;

	/**
	 * @brief
	 * The file storing the seeds data to import.
	 */
	std::string m_importSeeds;

	/**
	* @brief
	* The file storing the heightmap data to import.
	*/
	std::string m_importHeightmap;

	/**
	* @brief
	* Defines whether the map data have to be exported to external files or not.
	*/
	bool m_exportMapEnabled;

	/**
	* @brief
	* The file in which the seeds data have to be exported.
	*/
	std::string m_exportSeeds;

	/**
	* @brief
	* The file in which the heightmap data have to be exported.
	*/
	std::string m_exportHeightmap;

	/**
	 * @brief
	 * Defines whether the dynamic boids system should be instanciated or not.
	 */
	bool m_boidsEnabled;
    /**************************************************************************
     * End of global "defines".
     *************************************************************************/

	/**************************************************************************
	 * MapParser class "defines".
	 *************************************************************************/
	/**
	 * @brief
	 * The number of characters contained in the "comment-skipping" buffer of
	 * the parser.
	 */
	int m_parserBufferSize;
	/**************************************************************************
	 * End of MapParser class "defines".
	 *************************************************************************/
};

#endif
