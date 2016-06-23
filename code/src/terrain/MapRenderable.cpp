/**
 * @file MapRenderable.cpp
 *
 * @see MapRenderable.hpp
 */
#include "./../../include/terrain/MapRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"
#include "./../../include/terrain/MapUtils.hpp"
#include "./../../include/texturing/TexUtils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <omp.h>

#include <iostream>

/**
 * @brief
 * A macro aiming at correcting the floating point errors
 * of voro++ vertices computation.
 *
 * @param x The abscissa of the point which coordinates must be "corrected".
 * @param y The ordinate of the point which coordinates must be "corrected".
 */
#define COMPENSATE_COMPUTATION_ERROR(x,y)\
    if((x)<0.0) x = 0.0;\
    if((y)<0.0) y = 0.0;

MapRenderable::MapRenderable(
    ShaderProgramPtr shaderProgram, 
    MapGenerator& mapGenerator
)
    :   HierarchicalRenderable(shaderProgram),
        m_positions(0),
        m_minAltitude(0.0),
        m_scaleAltitude(0.0),
        m_mapGenerator(mapGenerator)
{

    // Geometry part : decomposing the voronoi diagram in triangles
    // and sending them
    sendVoronoiDiagram(mapGenerator);

    // Computing and binding the height map
    sendHeightMap();

    
    std::vector<std::string> filenames;
    std::string name;
    std::string extension = ".png";
    int minRes = 2;
    int maxRes = 8;
    // Send the textures for the map
    // Sea (ground)
    name = "../textures/shutter_texture_sand_sea";
    for (int i = minRes; i <= maxRes; ++i) {
	filenames.push_back(name + std::to_string(i) + extension);
    }
    sendMipMapTextures(filenames, &m_seaTexId);
    filenames.clear();
    // Beach
    name = "../textures/shutter_texture_sand_beach";
    for (int i = minRes; i <= maxRes; ++i) {
	filenames.push_back(name + std::to_string(i) + extension);
    }
    sendMipMapTextures(filenames, &m_sandTexId);
    filenames.clear();
    // Plains
    name = "../textures/shutter_texture_grass";
    for (int i = minRes; i <= maxRes; ++i) {
	filenames.push_back(name + std::to_string(i) + extension);
    }
    sendMipMapTextures(filenames, &m_plainsTexId);
    filenames.clear();
    // Lake
    name = "../textures/shutter_texture_lake";
    for (int i = minRes; i <= maxRes; ++i) {
	filenames.push_back(name + std::to_string(i) + extension);
    }    
    sendMipMapTextures(filenames, &m_lakeTexId);
    filenames.clear();
    // Mountain
    name = "../textures/shutter_texture_mountain";
    for (int i = minRes; i <= maxRes; ++i) {
	filenames.push_back(name + std::to_string(i) + extension);
    }
    sendMipMapTextures(filenames, &m_mountainTexId);
    filenames.clear();
    // Peak
    name = "../textures/shutter_texture_snow";
    for (int i = minRes; i <= maxRes; ++i) {
	filenames.push_back(name + std::to_string(i) + extension);
    }
    sendMipMapTextures(filenames, &m_peakTexId);
    filenames.clear();

    // Compute the masks
    sendMasks();
}

MapRenderable::~MapRenderable()
{
    /*
     * Disabling the GPU buffers used while rendering.
     */
    glcheck(glDeleteBuffers(1, &m_positionsBufferID));
    glcheck(glDeleteBuffers(1, &m_texCoordsID));
    glcheck(glDeleteBuffers(1, &m_heightmapID));

}

void MapRenderable::do_draw()
{
    /*
     * Drawing geometric data
     */

    // Position
    int positionLocation        = m_shaderProgram->getAttributeLocation("coord");
    
    // TexCoords
    int texCoordLocation        = m_shaderProgram->getAttributeLocation("texCoord");

    // Model matrix
    int modelLocation           = m_shaderProgram->getUniformLocation("modelMat");
    
    // Height map
    int heightmapLocation       = m_shaderProgram->getUniformLocation("heightMap");
    int heightMinLocation       = m_shaderProgram->getUniformLocation("heightMin");
    int heightScaleLocation     = m_shaderProgram->getUniformLocation("heightScale");

    // Tesselation parameters
    int tessLevelLocation       = m_shaderProgram->getUniformLocation("tessellationLevel");
    //int maxDistLocation         = m_shaderProgram->getUniformLocation("maxDist");
    
    // The texture scale parameter
    int scaleTextureLocation    = m_shaderProgram->getUniformLocation("scaleTexture");

    // Textures
    int seaTextureLocation      = m_shaderProgram->getUniformLocation("seaTex"); 
    int sandTextureLocation     = m_shaderProgram->getUniformLocation("sandTex"); 
    int plainsTextureLocation   = m_shaderProgram->getUniformLocation("plainsTex"); 
    int lakeTextureLocation     = m_shaderProgram->getUniformLocation("lakeTex"); 
    int mountainTextureLocation = m_shaderProgram->getUniformLocation("mountainTex"); 
    int peakTextureLocation     = m_shaderProgram->getUniformLocation("peakTex"); 
    
    // Masks (ie blending coefficients)
    // (x, y, z, w) = (sea, sand, plains, lake)
    int seaSandPlainsLakeMaskLocation = m_shaderProgram->getUniformLocation("seaSandPlainsLakeMask"); 
    // (x, y)       = (mountain, peak)
    int mountainPeakMaskLocation     = m_shaderProgram->getUniformLocation("mountainPeakMask");
    
    //Send material uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);

    /*
     * Sending the uniforms to their locations on the GPU.
     * Lone exception, the textures are simply bound here.
     */
    
    // Positions
    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_positionsBufferID));
        glcheck(glVertexAttribPointer(
                    positionLocation, 
                    3,
                    GL_FLOAT, 
                    GL_FALSE, 
                    0, 
                    (void*)0
                )
        );
    }

    // Texture coordinates
    if(texCoordLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(texCoordLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_texCoordsID));
        glcheck(glVertexAttribPointer(
                    texCoordLocation, 
                    2,
                    GL_FLOAT, 
                    GL_FALSE, 
                    0, 
                    (void*)0
                )
        );
    }

    // Model matrix
    if(modelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniformMatrix4fv(
                    modelLocation, 
                    1, 
                    GL_FALSE, 
                    glm::value_ptr(getModelMatrix())
                )
        );
    }

    // Height map as a texture
    if (heightmapLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_heightmapID));
        glcheck(glUniform1i(heightmapLocation, 0));
    }

    // Minimal height
    if (heightMinLocation != ShaderProgram::null_location)
    {
        glcheck(glUniform1f(heightMinLocation, m_minAltitude));
    }

    // Height scale
    if (heightScaleLocation != ShaderProgram::null_location)
    {
        glcheck(glUniform1f(heightScaleLocation, m_scaleAltitude));
    }

    // Tesselation level
    if (tessLevelLocation != ShaderProgram::null_location)
    {
        glcheck(glUniform1f(
                    tessLevelLocation, 
                    m_mapGenerator.m_mapParameters.getTessellationLevel()
                )
        );
    }

    
    // First texture mask (Sea, sand, plains, lake)
    if (seaSandPlainsLakeMaskLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE1));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_seaSandPlainsLakeMaskId));
        glcheck(glUniform1i(seaSandPlainsLakeMaskLocation, 1));
    }

    // Second mask (Mountain, peak)
    if (mountainPeakMaskLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE2));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_mountainPeakMaskId));
        glcheck(glUniform1i(mountainPeakMaskLocation, 2));
    }

    // Texture scale
    if (scaleTextureLocation != ShaderProgram::null_location)
    {
        glcheck(glUniform1f(scaleTextureLocation, 
			    m_mapGenerator.m_mapParameters.getLandTextureScaling()));
    }

    // Sea Texture
    if (seaTextureLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE3));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_seaTexId));
        glcheck(glUniform1i(seaTextureLocation, 3)); // ?
    }


    // Sand Texture
    if (sandTextureLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE4));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_sandTexId));
        glcheck(glUniform1i(sandTextureLocation, 4)); // ?
    }


    // Plains Texture
    if (plainsTextureLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE5));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_plainsTexId));
        glcheck(glUniform1i(plainsTextureLocation, 5)); // ?
    }


    // Lake Texture
    if (lakeTextureLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE6));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_lakeTexId));
        glcheck(glUniform1i(lakeTextureLocation, 6)); // ?
    }


    // Mountain Texture
    if (mountainTextureLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE7));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_mountainTexId));
        glcheck(glUniform1i(mountainTextureLocation, 7)); // ?
    }

    // Peak Texture
    if (peakTextureLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE8));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_peakTexId));
        glcheck(glUniform1i(peakTextureLocation, 8)); // ?
    }

    /*
     * Tessellating the map, and rendering it.
     */
    glcheck(glPatchParameteri(GL_PATCH_VERTICES, 3));
    glcheck(glDrawArrays(GL_PATCHES, 0, m_positions.size()));

    // Release texture 
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));
    /*
     * Disabling the buffers.
     */
    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if(texCoordLocation != ShaderProgram::null_location)
    { 
	glcheck(glDisableVertexAttribArray(texCoordLocation));
    }


}

void MapRenderable::do_animate(float time) 
{
}


void MapRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}


void MapRenderable::insertIntoList(
    std::list< std::pair<float, float> >& list,
    float x,
    float y,
    float centerX,
    float centerY
)
{
    /*
     * If the list is empty, just pushing back the new pair.
     * Otherwise, we have to determine the position to insert the pair to.
     *  In order to do so, we simply iterate over the pairs, comparing the
     *  orientation of the radial vector (with the center of the cell) of
     *  pair to insert with the latter's ones.
     *  The last comparison needs extra care, since we may have to push
     *  back the pair to insert if its orientation is greater than
     *  the one of the list's last pair.
     */
    if (list.empty()) {
        list.push_back(std::pair<float, float>(x, y));
    } else {
        auto current = list.begin();
        auto previous = current;
        auto last = list.end();
        float angle = atan2(y - centerY, x - centerX);
        float currentAngle;
        
        do {
            currentAngle = atan2(
                current->second - centerY,
                current->first - centerX
            );
            previous = current;
            current++;   
        } while ((current != last) && (angle > currentAngle));
        
        if (angle <= currentAngle) {
            list.insert(previous, std::pair<float, float>(x,y));
        } else {
            list.push_back(std::pair<float, float>(x,y));
        }
    }
}

/**
 * @brief
 * Static function aiming at finding if, considering a lake, a connexe lake, 
 * identified by "lakeID", has already been inserted into the associated
 * data structure (namely the vector storing information about lake connexity,
 * "m_lakesTriangles").
 * If "newLake" is set to "true" when given to the function, then no need for
 * fusionning lakes (since, if a connexe lake biome is found, it is the first
 * to be).
 * On the contrary, if "newLake" has been set to "false" by the current function
 * during a previous execution, and another connexe lake is found, the latter has
 * to be fused with the first found connexe lake.
 *
 * @param lakes A reference on the data structure containing information about
 * lakes connexity.
 * @param lakeID The ID of the connexe lake to search for.
 * @param resultPair A pointer on the pointer in which the address of the pair
 * of "lakes" to consider will be stored, if the connexe lake is found.
 * @param newLake A boolean representing "the connexe lake has been found".
 */
static void findList(
    std::list< std::pair< std::vector<int>, std::vector<glm::vec3> > >& lakes,
    int lakeID,
    std::pair< std::vector<int>, std::vector<glm::vec3> >** resultPair,
	bool& newLake
)
{
    for (
        auto itLakes = lakes.begin(); 
        itLakes != lakes.end(); 
        itLakes++
    ) {
        for (
            auto itLakeIDs = itLakes->first.begin();
            itLakeIDs != itLakes->first.end();
            itLakeIDs++
        ) {
			if (*itLakeIDs == lakeID) {
				if (newLake) {
					*resultPair = &(*itLakes);
					newLake = false;
				} else {
					/*
					 * If the connexe Lake biome is within the current connexe
					 * lake, nothing to do.
					 * Otherwise, we have to fuse the found connexe lake with
					 * the current one. In other words, to push back the content
					 * of the associated vectors into those of the final lake, and
					 * remove of the list the pair which has been fused. 
					 */
					if (*resultPair != &(*itLakes)) {
						for (
							auto intIt = itLakes->first.begin();
							intIt != itLakes->first.end();
							intIt++
						) {
							(*resultPair)->first.push_back(*intIt);
						}
						for (
							auto glmIt = itLakes->second.begin();
							glmIt != itLakes->second.end();
							glmIt++
						) {
							(*resultPair)->second.push_back(*glmIt);
						}
						lakes.erase(itLakes);
					}
				}
				/*
				 * Since a biome lake can be involded in only one connexe lake,
				 * if the considered biome lake has been found, no need to iterate
				 * over the remaining connexe lakes.
				 */
				return;
			}
        }
    }
}

void MapRenderable::sendVoronoiDiagram(MapGenerator& mapGenerator) 
{
   
    /*
     * Iterating on each cell in order to obtain the coordinates of each
     * of its vertices.
     * This will be used to draw the cells.
     * We are going to store in a list, a list by cell, containing the
     * vertices of the latter.
     * Since we are interested in bi-dimensional Voronoi diagrams, the
     * vertices are going to be stored as pairs of float/coordinates.
     */
    std::list< std::list< std::pair<float, float> >* > listOfVertices;

    for (
        auto seedsIterator = m_mapGenerator.seeds.begin();
        seedsIterator != m_mapGenerator.seeds.end();
        seedsIterator++
    ) {
        /*
         * First, we have to compute the vertices of the considered cell.
         * Thanks to one of the voro++ primitives, we can obtain a vector
         * containing all the concatened coordinates of the cell's vertices.
         */
        std::vector<double> verticesCoordinates;
        seedsIterator->getCell()->vertices(
            (double)seedsIterator->getX(),
            (double)seedsIterator->getY(),
            0.0,
            verticesCoordinates
        );

        /*
         * Then, we parse the computed coordinates to create the vectors of
         * points.
         */
        auto cellVertices = new std::list< std::pair<float, float> >();
        for (
            auto iterator = verticesCoordinates.begin(); 
            iterator != verticesCoordinates.end(); 
        ) {
            /*  
             * As voro++ is working with a tridimensional space, and we are
             * interested in plane diagrams, we have to keep the only the
             * vertices in the positive "half-3Dspace".
             * Indeed, we set to 0.0 all the z-coordinates of the seeds, so
             * as to obtain a "plane 3D Voronoi diagram" center on the origin.
             * Thus, the plane (Oxy) is a median plane of the global diagram,
             * and the 3D diagram can be view has a solid obtained by extruding
             * the 2D diagram shape.
             * That is why, by only selecting the vertices of one of the half-spaces,
             * we obtain the shape of the 2D diagram.
             */
            float x = *(iterator)++;
            float y = *(iterator)++;
            float z = *(iterator)++;

            if (z >= 0.0) {
                COMPENSATE_COMPUTATION_ERROR(x,y);
                insertIntoList(
                    *cellVertices, 
                    x,
                    y,
                    seedsIterator->getX(), 
                    seedsIterator->getY() 
                );
            }
        }

        listOfVertices.push_back(cellVertices);
    }

    /*
     * Iterating on each seed in order to fill the different buffers with the
     * necessary data to render the map.
     */
    auto seedsIt = m_mapGenerator.seeds.begin();
    int count = 0;
    for (
        auto listIt = listOfVertices.begin();
        listIt != listOfVertices.end();
        listIt++, seedsIt++, count++
    ) {
        /*
         * Getting the centroid and the biome of the cell, since all the 
         * triangles will connect to the former, and be of the type of the
         * latter.
         */
        glm::vec3 p1;
        glm::vec3 p2;
        float cX = seedsIt->getCentroidX();
        float cY = seedsIt->getCentroidY();
        glm::vec3 centroid(
            cX,
            cY,
            m_mapGenerator.getHeight(cX, cY)
        );

        /*
         * In order to merge connexe lakes, we have to iterate over their
         * neighbourhoods, and insert their triangles into shared vectors.
         */
        bool newLake = true;
        std::vector<glm::vec3>* lakeVector = NULL;
        if (seedsIt->getBiome() == Lake) {
            /*
             * Iterating over the lake's neighbourhood.
             */
            std::vector<int> neighbours;
            seedsIt->getCell()->neighbors(neighbours);

            std::pair< 
                std::vector<int>, 
                std::vector<glm::vec3> 
            >* insertPair;

			for (
				auto neighboursIt = neighbours.begin();
				neighboursIt != neighbours.end();
				neighboursIt++
				)
			{
				/*
				 * If one of its neighbour has already been inserted into the
				 * lakes list, then the considered lake is connexe to its
				 * neighbour.
				 * If a connexe lake has already been found, then checking the 
				 * other neighbours of the considered lake in order to detect 
				 * secondary connexity.
				 */
				findList(m_lakesTriangles, *neighboursIt, &insertPair, newLake);
			}
            /*
             * Otherwise, we have to insert the lake into the list of lakes.
             */
            if (newLake) {
                m_lakesTriangles.push_back(
                    std::pair< std::vector<int>, std::vector<glm::vec3> >()
                );
                insertPair = &(m_lakesTriangles.back());
            }
            insertPair->first.push_back(count);
            lakeVector = &(insertPair->second);
        }

        auto current = (*listIt)->begin();
        auto previous = current;
        current++;

        /*
         * Iterating on each vertex associated with the seed, that is to say
         * the cell, and constructing the associated triangles.
         * In other words, each pair of consecutive vertices is linked with the
         * center of the cell to construct a triangle.
         */
        while (current != (*listIt)->end()) {
            /*
             * Initialisation of the computing of the points.
             */
            p1.x = previous->first;
            p1.y = previous->second; 
            p1.z = m_mapGenerator.getHeight(p1.x, p1.y);
            p2.x = current->first;
            p2.y = current->second; 
            p2.z = m_mapGenerator.getHeight(p2.x, p2.y);

            /*
             * Pushing the information associated with the first point to the
             * buffers.
             */
            m_positions.push_back(p1);
            m_texCoords.push_back(glm::vec2(p1)/m_mapGenerator.mapSize);
            /*
             * Pushing the information associated with the second point to the
             * buffers.
             */
            m_positions.push_back(p2);
            m_texCoords.push_back(glm::vec2(p2)/m_mapGenerator.mapSize);

            /*
             * Pushing the information associated with the centroid to the
             * buffers.
             */
            m_positions.push_back(centroid);
            m_texCoords.push_back(glm::vec2(centroid)/m_mapGenerator.mapSize);

            /*
             * Pushing the lakes' triangles, possibly extended or shrinked thanks to the dedicated
             * coefficient.
             */
            if (lakeVector != NULL) {
                float coeff = mapGenerator.m_mapParameters.getLakesExtension();
                lakeVector->push_back(glm::vec3(coeff*glm::vec2(p1-centroid)+glm::vec2(centroid), p1.z));
                lakeVector->push_back(glm::vec3(coeff*glm::vec2(p2-centroid)+glm::vec2(centroid), p2.z));
                lakeVector->push_back(centroid);
            }

            previous = current;
            current++;
        }

        /*
         * The last triangle between the last and first vertices, and the c
         * entroid has to be created "manually", after the loop.
         */
        p1.x = (*listIt)->front().first;
        p1.y = (*listIt)->front().second;
        p1.z = m_mapGenerator.getHeight(p1.x, p1.y);
        
        m_positions.push_back(p2);
        m_texCoords.push_back(glm::vec2(p2)/m_mapGenerator.mapSize);

        m_positions.push_back(p1);
        m_texCoords.push_back(glm::vec2(p1)/m_mapGenerator.mapSize);
     
        m_positions.push_back(centroid);
        m_texCoords.push_back(glm::vec2(centroid)/m_mapGenerator.mapSize);

        if (lakeVector != NULL) {
            float coeff = mapGenerator.m_mapParameters.getLakesExtension();
            lakeVector->push_back(glm::vec3(coeff*glm::vec2(p1-centroid)+glm::vec2(centroid), p1.z));
            lakeVector->push_back(glm::vec3(coeff*glm::vec2(p2-centroid)+glm::vec2(centroid), p2.z));
            lakeVector->push_back(centroid);
        }
    }


    /*
     * Deleting the dynamically allocated memory.
     */
    auto listIt = listOfVertices.begin();
    auto previous = listIt;
    listIt++;

    while (listIt != listOfVertices.end()) {
        (*previous)->clear();
        delete (*previous);
        previous = listIt;
        listIt++;
    }

    /*
     * Last vector has to deallocated "manually" since the loop
     * stops before deleting it ("listOfVertices.end()" is
     * encountered before deletion of the vector pointed by
     * "previous").
     */
    (*previous)->clear();
    delete (*previous);

    /*
     * Creation of the buffers on the GPU, and storing their IDs.
     */
    glGenBuffers(1, &m_positionsBufferID);
    glGenBuffers(1, &m_texCoordsID);

    /*
     * Activating buffers and sending data.
     */
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_positionsBufferID));
    glcheck(glBufferData(
                GL_ARRAY_BUFFER, 
                m_positions.size()*sizeof(glm::vec3), 
                m_positions.data(), 
                GL_STATIC_DRAW
            )
    );

    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_texCoordsID));
    glcheck(glBufferData(
                GL_ARRAY_BUFFER, 
                m_texCoords.size()*sizeof(glm::vec2), 
                m_texCoords.data(), 
                GL_STATIC_DRAW
            )
    );
}





void MapRenderable::sendHeightMap() {
    /*
     * Creating and computing the 2D texture representing the heightmap.
     */
    int heightmapScaling    = m_mapGenerator.m_mapParameters.getHeightmapScaling();
    int mapSize             = (int) m_mapGenerator.mapSize;
    int effMapSize          = mapSize*heightmapScaling;
    int effMapDimension     = effMapSize + 1;
    float* heightMap        = new float [4*effMapDimension*effMapDimension];

    /*
     * We sample the heightmap within the texture according to a 
     * (1 pixel = <heightMapScaling = 2> meters) scale.
     * On top of that, we have to retain the maximum and minimum altitudes
     * so as to apply a scaling during the Tessellation step (which
     * prevent us from using OpenMP to parallelize this loop).
     */
    float maxAltitude = 0.0;
    for (int i = 0; i < effMapSize; i++) {
        for (int j = 0; j < effMapSize; j++) {
            
            float effI = (float)i / (float)heightmapScaling;
            float effJ = (float)j / (float)heightmapScaling;
            float altitude = m_mapGenerator.getHeight(effI, effJ);
            if (altitude < m_minAltitude) {
                m_minAltitude = altitude;
            } else if (altitude > maxAltitude) {
                maxAltitude = altitude;
            }
            // Computing the normal
            float delta = 0.1f / (float)heightmapScaling;
            float dzx   = (m_mapGenerator.getHeight(effI + delta, effJ) - altitude);
            float dzy   = (m_mapGenerator.getHeight(effI, effJ + delta) - altitude);
            // The normal
            glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(delta, 0.0f,  dzx),
                                                         glm::vec3(0.0f,  delta, dzy)));
            // Transforming the normal to fit in the texture
            normal = (normal*0.5f) + glm::vec3(0.5f); 

	    heightMap[(i+j*effMapDimension)*4]     = normal.x;
            heightMap[(i+j*effMapDimension)*4 + 1] = normal.y;
            heightMap[(i+j*effMapDimension)*4 + 2] = normal.z;
            heightMap[(i+j*effMapDimension)*4 + 3] = altitude;
        }
    }
    // Special case : copying for the last row and column
    #pragma omp parallel for
    for (int i = 0; i < effMapDimension; i++) {
        heightMap[(i+(effMapSize*effMapDimension))*4]     = heightMap[(i+((effMapSize-1)*effMapDimension))*4];
        heightMap[(i+(effMapSize*effMapDimension))*4 + 1] = heightMap[(i+((effMapSize-1)*effMapDimension))*4 + 1];
        heightMap[(i+(effMapSize*effMapDimension))*4 + 2] = heightMap[(i+((effMapSize-1)*effMapDimension))*4 + 2];
        heightMap[(i+(effMapSize*effMapDimension))*4 + 3] = heightMap[(i+((effMapSize-1)*effMapDimension))*4 + 3];
    }
    #pragma omp parallel for
    for (int j = 0; j < effMapDimension; j++) {
        heightMap[(effMapSize+(effMapDimension*j))*4]     = heightMap[((effMapSize-1)+(effMapDimension*j))*4];
        heightMap[(effMapSize+(effMapDimension*j))*4 + 1] = heightMap[((effMapSize-1)+(effMapDimension*j))*4 + 1];
        heightMap[(effMapSize+(effMapDimension*j))*4 + 2] = heightMap[((effMapSize-1)+(effMapDimension*j))*4 + 2];
        heightMap[(effMapSize+(effMapDimension*j))*4 + 3] = heightMap[((effMapSize-1)+(effMapDimension*j))*4 + 3];
    }
    
    /*
     * Normalizing the heightmap, since the OpenGL tessellation shaders need
     * texture coordinates between 0 and 1.
     * That is why the scaling is needed, in order to have a final map with
     * altitudes corresponding to the original heightmap's ones.
     */
    m_scaleAltitude = maxAltitude - m_minAltitude;
    #pragma omp parallel for
    for (int i = 0; i < effMapDimension*effMapDimension; i++) {
        heightMap[i*4 + 3] -= m_minAltitude;
		heightMap[i*4 + 3] /= m_scaleAltitude;
    }
    
     /*
     * Creating the texture on the GPU.
     */
    glGenTextures(1, &m_heightmapID);

    /*
     * Binding the texture.
     */
    glBindTexture(GL_TEXTURE_2D, m_heightmapID);

    /*
     * Setting the texture's parameters.
     */
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    /*
     * Sending the texture to the GPU.
     */
    glcheck(
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA32F,
            effMapDimension,
            effMapDimension,
            0,
            GL_RGBA,
            GL_FLOAT,
            (const GLvoid*) heightMap
        )
    );

    /*
     * Releasing the texture.
     */
    glBindTexture(GL_TEXTURE_2D, 0);
    delete [] heightMap;

}


void MapRenderable::sendMasks() {
    
    // Computing the parameters
    int heightmapScaling    = m_mapGenerator.m_mapParameters.getHeightmapScaling();
    int mapSize             = (int) m_mapGenerator.mapSize;
    int effMapSize          = mapSize*heightmapScaling;
    int effMapDimension     = effMapSize + 1;
    
    // Allocating the masks
    // Sea-Sand-Plains-Lake
    float* maskSSPL         = new float [4*effMapDimension*effMapDimension];
    // Mountain-Peak
    float* maskMP           = new float [2*effMapDimension*effMapDimension];


    // Variables to store the local neighbourhood
    float seaNeighbour;
    float sandNeighbour;
    float plainsNeighbour;
    float lakeNeighbour;
    float mountainNeighbour;
    float peakNeighbour;

    float increment;
    
    int neighbourhoodWidth   = 5;
    float neighbourhoodSize  = 1.0f;

    // Computing
    for (int i = 0; i < effMapSize; i++) {
        for (int j = 0; j < effMapSize; j++) {

	    
	    if (j == 0) {
		// We start on a now column : reset of the neighbourhood
		// Reset to 0
		seaNeighbour      = 0.0f;
		sandNeighbour     = 0.0f;
		plainsNeighbour   = 0.0f;
		lakeNeighbour     = 0.0f;
		mountainNeighbour = 0.0f;
		peakNeighbour     = 0.0f;
		neighbourhoodSize = 0.0f;
		// Counting the new neighbourhood
		for (int ni = MAX(0, i - neighbourhoodWidth);
		     ni < MIN(effMapSize, i + neighbourhoodWidth);
		     ++ni) {
		    for (int nj = MAX(0, j - neighbourhoodWidth);
			 nj < MIN(effMapSize, j + neighbourhoodWidth);
			 ++nj) {
			increment = countBiome(m_mapGenerator.m_mapParameters,
					       m_mapGenerator.biomeMap[ni+nj*effMapSize], 1,
					       &seaNeighbour,      &sandNeighbour,
					       &plainsNeighbour,   &lakeNeighbour,
					       &mountainNeighbour, &peakNeighbour);
			neighbourhoodSize += increment;
		    }
		}
	    } else {
		// We are on the same column
		if (j > neighbourhoodWidth) {
		    // We are far enough in the column to start forgetting a row
		    int nj = j - neighbourhoodWidth - 1;
		    for (int ni = MAX(0, i - neighbourhoodWidth);
			 ni < MIN(effMapSize, i + neighbourhoodWidth);
			 ++ni) {
			increment = countBiome(m_mapGenerator.m_mapParameters,
					       m_mapGenerator.biomeMap[ni+nj*effMapSize], -1,
					       &seaNeighbour,      &sandNeighbour,
					       &plainsNeighbour,   &lakeNeighbour,
					       &mountainNeighbour, &peakNeighbour);
			
			neighbourhoodSize += increment;
		    }
		}
		if (j < effMapSize - neighbourhoodWidth) {
		    // We are not close to the end of the column, we can add the next row
		    int nj = j + neighbourhoodWidth;
		    for (int ni = MAX(0, i - neighbourhoodWidth);
			 ni < MIN(effMapSize, i + neighbourhoodWidth);
			 ++ni) {
			increment = countBiome(m_mapGenerator.m_mapParameters,
					       m_mapGenerator.biomeMap[ni+nj*effMapSize], 1,
					       &seaNeighbour,      &sandNeighbour,
					       &plainsNeighbour,   &lakeNeighbour,
					       &mountainNeighbour, &peakNeighbour);
			neighbourhoodSize += increment;
		    }
		}    
	    }

	    // First mask
	    maskSSPL[(i+j*effMapDimension)*4]     = seaNeighbour      / neighbourhoodSize; 
            maskSSPL[(i+j*effMapDimension)*4 + 1] = sandNeighbour     / neighbourhoodSize; 
            maskSSPL[(i+j*effMapDimension)*4 + 2] = plainsNeighbour   / neighbourhoodSize; 
            maskSSPL[(i+j*effMapDimension)*4 + 3] = lakeNeighbour     / neighbourhoodSize;
	    // Second mask
	    maskMP[(i+j*effMapDimension)*2]       = mountainNeighbour / neighbourhoodSize;
	    maskMP[(i+j*effMapDimension)*2 + 1]   = peakNeighbour     / neighbourhoodSize;
	}
    }
    // Copying the last row and columns
    #pragma omp parallel for
    for (int i = 0; i < effMapDimension; i++) {
	// First mask
        maskSSPL[(i+(effMapSize*effMapDimension))*4]     = maskSSPL[(i+((effMapSize-1)*effMapDimension))*4];
        maskSSPL[(i+(effMapSize*effMapDimension))*4 + 1] = maskSSPL[(i+((effMapSize-1)*effMapDimension))*4 + 1];
        maskSSPL[(i+(effMapSize*effMapDimension))*4 + 2] = maskSSPL[(i+((effMapSize-1)*effMapDimension))*4 + 2];
        maskSSPL[(i+(effMapSize*effMapDimension))*4 + 3] = maskSSPL[(i+((effMapSize-1)*effMapDimension))*4 + 3];
	// Second mask
	maskMP[(i+(effMapSize*effMapDimension))*2]       = maskMP[(i+((effMapSize-1)*effMapDimension))*2];
        maskMP[(i+(effMapSize*effMapDimension))*2 + 1]   = maskMP[(i+((effMapSize-1)*effMapDimension))*2 + 1];
    }

    #pragma omp parallel for
    for (int j = 0; j < effMapDimension; j++) {
	// First mask
        maskSSPL[(effMapSize+(effMapDimension*j))*4]     = maskSSPL[((effMapSize-1)+(effMapDimension*j))*4];
        maskSSPL[(effMapSize+(effMapDimension*j))*4 + 1] = maskSSPL[((effMapSize-1)+(effMapDimension*j))*4 + 1];
        maskSSPL[(effMapSize+(effMapDimension*j))*4 + 2] = maskSSPL[((effMapSize-1)+(effMapDimension*j))*4 + 2];
        maskSSPL[(effMapSize+(effMapDimension*j))*4 + 3] = maskSSPL[((effMapSize-1)+(effMapDimension*j))*4 + 3];
	// Second mask
	maskMP[(effMapSize+(effMapDimension*j))*2]       = maskMP[((effMapSize-1)+(effMapDimension*j))*2];
        maskMP[(effMapSize+(effMapDimension*j))*2 + 1]   = maskMP[((effMapSize-1)+(effMapDimension*j))*2 + 1];
    }

    // Sending the two masks

    // First mask
    // Creation
    glGenTextures(1, &m_seaSandPlainsLakeMaskId);
    // Bind
    glBindTexture(GL_TEXTURE_2D, m_seaSandPlainsLakeMaskId);
    // Parameters
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    // Sending
    glcheck(glTexImage2D(GL_TEXTURE_2D,
			 0,
			 GL_RGBA32F,
			 effMapDimension,
			 effMapDimension,
			 0,
			 GL_RGBA,
			 GL_FLOAT,
			 (const GLvoid*) maskSSPL));

    // Second mask
    // Creation
    glGenTextures(1, &m_mountainPeakMaskId);
    // Bind
    glBindTexture(GL_TEXTURE_2D, m_mountainPeakMaskId);
    // Parameters // Useless here ?
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    // Sending
    glcheck(glTexImage2D(GL_TEXTURE_2D,
			 0,
			 GL_RG32F,      
			 effMapDimension,
			 effMapDimension,
			 0,
			 GL_RG,
			 GL_FLOAT,
			 (const GLvoid*) maskMP));

    // Releasing the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Freeing the masks
    delete [] maskSSPL;
    delete [] maskMP;

}

std::list< 
    std::pair<
        std::vector<int>, 
        std::vector<glm::vec3> 
    >
>& MapRenderable::getLakesTriangles()
{
    return m_lakesTriangles;
}
