# Details about the **_mapData_** directory
The **_mapData_** repository has a main role in the (procedural) map generations, and some of its intern mechanism might not be "crystal-clear" understandable.
This **_README_** aims at popularising the role and the functioning of the former. 
## Purpose and operating of the **_mapData_** directory
### Purpose
In a few words, the **_mapData_** directory contains the parameter files associated with the map generation, and the map import/export files. 

On the one hand, concerning the parameters involved into the map generation, they spread from the size of the map to the parameters entailed in the probability laws which decide the biome type of a Voronoi cell. Those parameters truly determine the final appearance and coherence of the terrain, since they have a direct effect upon the implemented rules entailed in the map generation.  
If the Ecosystem Generator were a black box, the parameters would be the user interface with what is within the box.

On the other hand, the map import/export is a key feature for development, and a way to confine the hazard part of the procedural terrain generation :
* **Concerning development**: let's say you notice a map generation bug while you are executing the simulator, being able to export the map data associated with the bug allows you to import it afterwards, while testing the efficiency of a patch (remember that with a procedural generation, obtaining _randomly_ the same map twice has a probability really near zero);
* **Concerning hazard confinement**: let's say you would really like to have a tridimensional version of the bidimensional map you have just drawn. By imposing a matching underlaying Voronoi diagram to the generator, you can achieve it easily.

Lastly, the user may want to test different settings and keep track of them. The **_mapData_** is the perfect place to do so.

### Operating
First, it is **really important** to make sure that there is a file named **_MapParameters.json_** at the root level of **_mapData_** directory.  
Indeed, this is the configuration file parsed by the Ecosystem Generator to retrieve its parameters (it goes without saying that the concerned file must comply with the JSon file format and contain all the necessary parameters - see the list of the latter below).

In the same way, the map import/export files to use should be at the root level of the **_mapdata_** directory (see below for details about import/export). The name of the files to use must be specified in the JSon parameter file.

Note that a copy of the original **_MapParameters.json_** (the one used by default, and natively at the root level of **_mapData_**) is available in the **_Original_** directory.

## Why using parsed parameter files ?
The main reasons are **simplicity** and **efficiency**. 

As a matter of fact, when you _hardcode_ the values of the parameters directly in the source/header files, you have to compile your project each time you change the value of one of the latter.  
Yet, compiling such a project may take up to several minutes and the parameters adjustment phase can easily require more than a hundred tries...  
Thus, at the scale of the project, you will have lose hours, if not days, waiting for your code to be compiled.

However, if the parameters are parsed from a file, you can modify them without re-compiling, since the code involved in the parsing phase has not changed.  
What is more, if you have a parsing-dedicated entity (for instance, our **MapParameters** class), adding a parameter to simulator will not imply to modify several function declarations just to comply by the variables scope definition.  
Indeed, you just have to add a member to the associated entity, which should already be reachable in the scope you need it.

## Exporting and importing maps
### Global overview
In order to export (and thus import) a generated map, some of the generated data associated with the map have to be saved:
* The plane position of the seeds used to build the underlayon Voronoi diagram;
* The biome type associated with the seeds;
* The sampled heightmap used to rendering;
* The JSon parameter file.

Those data, parameters file excluded, may be separated in two distinct parts, that is to say exported toward two different files:
* **_seeds_data_**: this file contains the seeds position and the biome types associated to them;
* **_heightmap_data_**: this file contains the sampled heights used to rendering the terrain.

Note that the names of those files is not compulsory (the used names are their default ones), and the former can be set in the **_MapParameters.json_** file (see the how-to-use below for more details).

When exporting, a timestamped directory is created in the **_mapData_** directory, in which the two above files are created and the JSon parameter file is copied. 

### Files format
So as to be able to analyse the exported data (when tracking a bug for example) and/or create particular map (when testing a feature which requires a specific topology/geometry), it is important that the file format used for the export file is user-friendly.  
That is why we designed a very simple file format, which allows comments use. Each comment (or line of a multi-line comment) must begin with the character **#**, and any character on the same line and at its right is considered as part of the comment.  
On top of that, spaces, tabulations, carriage returns and line feeds are considered as white-spaces characters when parsed by the **MapParser** class.

Let's zoom at each file's intern structure:
* **_seeds_data_**: Each line corresponds to a seeds and contains, in order, its abscissa, its ordinate and its biome type.  
It is **necessary** that the number of seeds in the file matches the number of seeds parameter in the associated **_MapParameters.json_** file;
```bash
####### Example of seeds export file.
# Seeds' data:
504.326 500.914 Peak
507.163 490.646 Mountain # Seed N°2's biome type is "Mountain".
487.473     520.112     Peak # Tabulation and (CR)LF are considered as white-space characters.

524.612 524.17 Mountain
496.142 464.324 Peak
505.565 542.012 Mountain
...
```
* **_heightmap_data_**: Each line corresponds to a sampled point of the heightmap and contains, in order, its abscissa, its ordinate and its height.  
It is **necessary** that the number of points in the file matches the effective number of points of the sampled heightmap, which can be computed as **_(mapSize x heightmapScaling)²_**, with **_mapSize_** and **_heightmapScaling_** being parameters in the associated **_MapParameter.json_** file.
```bash
####### Example of heightmap export file.
# Heightmap's data:
0 0 -20.3741
0 1 -20.372
0 2 -20.3603
0 3 -20.3368
0 4 -20.3045
0 5 -20.2695
0 6 -20.2379
0 7 -20.2132
...
```
### How to use import/export
##### Export
In order to export a map, the **exportMapEnabled** parameter must be set to **true**. Then, when visualizing the generated terrain in the viewer/window associated with the Ecosystem Generator, simply press the [**_e_**] key of your keyboard.  
You will be notified of both the export's beginning and success, and the created/copied files will be available in a created timestamped directory within **_mapData_**.  
Note that the created files' names can be set through the parameters **exportSeeds** and **exportHeightmap**.

##### Import
First things first, the boolean parameters **importingSeeds** and **importingHeightmap** allow you to specify whether you to import the map data or not.  
The imported files names are set through the parameters **importSeeds** and **importHeightmap**, and the corresponding files must be at the root level of **_mapData_**, as previously mentionned.  
As it was also previously mentionned, **do not forget** to supply a **_MapParameter.json_** file which parameters match the data of the imported files (mainly the number of seeds and the number of sampled points for the heightmap).

## List of **all** the terrain generation parameters
### Global parameters
* **mapSize**: Defines the size of the square map;
* **importingSeeds**: Defines whether the seeds' data are import from external files or not;
* **importingHeightmap**: Defines whether the heightmap data are import from external files or not;
* **importSeeds**: The file storing the seeds data to import;
* **importHeightmap**: The file storing the heightmap data to import;
* **exportMapEnabled**: Defines whether the map data have to be exported to external files or not;
* **exportSeeds**: The file in which the seeds data have to be exported;
* **exportHeightmap**: The file in which the heightmap data have to be exported;
* **boidsEnabled**: Defines whether the dynamic boids system should be instanciated or not.

### MapGenerator class parameters
* **nbSeeds**: Defines the default number of seeds to be generated by the VoronoiSeedsGenerator;
* **nbSubdivision**: Defines the default number of subdivisions for each dimension of the grid used within the VoronoiSeedsGenerator;
* **nbSeedsMaxSubdiv**: Defines the default number of seeds by subdivisions of the grid used within the VoronoiSeedsGenerator;
* **distMin**: Defines the default minimal distance between two generated seeds.

### MapParser class parameters
* **parserBufferSize**: The number of characters contained in the "comment-skipping" buffer of the parser.

### Biome class parameters
* **heightMinSea**: Defines the minimal height associated with a biome of sea;
* **heightMaxSea**: Defines the maximal height associated with a biome of sea;
* **heightMinLake**: Defines the minimal height associated with a biome of lake;
* **heightMaxLake**: Defines the maximal height associated with a biome of lake;
* **heightMinInnerBeach**: Defines the minimal height associated with a biome "InnerBeach";
* **heightMaxInnerBeach**: Defines the maximal height associated with a biome "InnerBeach";
* **heightMinOuterBeach**: Defines the minimal height associated with a biome "OuterBeach";
* **heightMaxOuterBeach**: Defines the maximal height associated with a biome "OuterBeach";
* **heightMinPlains**: Defines the minimal height associated with a biome "Plains";
* **heightMaxPlains**: Defines the maximal height associated with a biome "Plains";
* **heightMinMountain**: Defines the minimal height associated with a biome "Mountain";
* **heightMaxMountain**: Defines the maximal height associated with a biome "Mountain";
* **heightMinPeak**: Defines the minimal height associated with a biome "Peak";
* **heightMaxPeak**: Defines the maximal height associated with a biome "Peak";
* **scaleLimitInfluence**: A scale parameter to limit the inluence of a biome on other biomes;
* **biomeDepthMin**: Defines the minimal number of recursive subdivisions in the QuadTree used to generate the HeightMap;
* **biomeDepthMax**: Defines the maximal number of recursive subdivisions in the QuadTree used to generate the HeightMap.

### BiomeRepartition class parameters
* **landBlendingCoefficient**: When deciding if a biome is going to be a land biome, we take into account both its distance to the center of the map and the proportion of its neighbours being land biomes. Defines the coefficient used to ponderate the importance of the former in comparison with the latter;
* **lakeGeometricPicking**: Defines the parameter of the geometric law used to generate Lake biomes;
* **lakeProbTransform**: Defines the probability that a Lake-eligible biome is effectively transformed into a Lake biome;
* **lakePositiveInfluence**: When a biome is transformed into a Lake, it has a special influence on its neighbours. As a matter of fact, it fosters the fact that its Lake-eligible neighbours are transformed into Lake too. Concretely, it defines the "bonus" probability added to the initial one when the previously described situation occurs;
* **mountainGeometricPicking**: Defines the parameter of the geometric law used to generate Mountain biomes;
* **mountainProbTransform**: Defines the probability that a Mountain-eligible biome is effectively transformed into a Mountain biome;
* **mountainMaxTry**: Defines the maximum number of iterations of the geometric law used to generate Mountain biomes. If this maximum is reached, then we stop trying to generate mountains.

### VoronoiSeedsGenerator class parameters
* **vsgWidth**: Default width of the constrained plane in which the seeds are generated;
* **vsgHeight**: Default height of the constrained plane in which the seeds are generated;
* **vsgNbSeeds**: Default number of seeds to generate;
* **vsgWidthSub**: Default width of a subdivision of the grid used to tessellate the constrained plane the seeds generation takes place within;
* **vsgHeightSub**: Default height of a subdivision of the grid used to tessellate the constrained plane the seeds generation takes place within;
* **vsgMaxSeedsBySub**: Default maximum of the number of seeds by subdivision of the grid used to tessellate the constrained plane the seeds generation takes place within;
* **vsgMinDist**: Default minimum distance between two generated seeds.

### HeightTree class parameters
* **detectionThreshold**: Determining if a point is already in the tree before inserting it, we have to compare the position of the point to insert with the positions of all the already inserted points. However, when comparing floats, such as the coordinates of the points, the computation error has to be taken into account (so as to avoid false negative). This threshold aims precisely at that : if the distance between two points is lesser than the former, the two points are considered equal;
* **heightBlendingCoefficient**: When determining the height associated with a point, we iterate recursively from the root of the tree to the corresponding node of the deepest level of subdivision. Each level contributes to the global height, but their contribution is recursively reduced by applying the defined blending coefficient.

### MapRenderable class parameters
* **tessellationLevel**: Multiplicative factor used in the Tessellation shaders so as to set the tessellation level for both the outer and inner tessellations;
* **heightmapScaling**: Scaling factor used to determine the precision (ie. the resolution) of the texture storing the heightmap. In other word, the resolution of the latter is equal to "mapSize*heightmapScaling";
* **landTextureScaling**: Defines the scaling for the texture application for the land;
* **seaTextureScaling**: Defines the scaling for the texture application for the sea;
* **lakeTextureScaling**: Defines the scaling for the texture application for the lakes.

### LakeRenderable class parameters
* **lakesExtension**: Defines the extension coefficient applied to the lakes renderables.

### Texture parameters
* **seaTextureExtent**: Defines the extent of a sea texture on its neighbour;
* **sandTextureExtent**: Defines the extent of a sand texture on its neighbour;
* **plainsTextureExtent**: Defines the extent of a plains texture on its neighbour;
* **lakeTextureExtent**: Defines the extent of a lake texture on its neighbour;
* **mountainTextureExtent**: Defines the extent of a mountain texture on its neighbour;
* **peakTextureExtent**: Defines the extent of a peak texture on its neighbour.
