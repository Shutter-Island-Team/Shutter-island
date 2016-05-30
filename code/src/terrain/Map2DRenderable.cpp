/**
 * @file Map2DRenderable.cpp
 *
 * @see Map2DRenderable.hpp
 */
#include "./../../include/terrain/Map2DRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

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

Map2DRenderable::Map2DRenderable(
    ShaderProgramPtr shaderProgram, 
    MapGenerator& mapGenerator
)
    :   Renderable(shaderProgram),
        m_positions(0),
        m_colors(0),
        m_normals(0),
        m_mapGenerator(mapGenerator)
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
    for (
        auto listIt = listOfVertices.begin();
        listIt != listOfVertices.end();
        listIt++, seedsIt++
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
            0.0
        );
        glm::vec4 cellColor = biomeColor(seedsIt->getBiome());

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
            p1.z = 0.0;
            p2.x = current->first;
            p2.y = current->second; 
            p2.z = 0.0;

            /*
             * Computing the normal, since it is the same for all the points
             * of the triangle.
             */
            glm::vec3 triangleNormal(
                glm::normalize(glm::cross(p2-p1, centroid-p1))    
            );

            /*
             * Pushing the information associated with the first point to the
             * buffers.
             */
            m_positions.push_back(p1);
            m_colors.push_back(cellColor);
            m_normals.push_back(triangleNormal);

            /*
             * Pushing the information associated with the second point to the
             * buffers.
             */
            m_positions.push_back(p2);
            m_colors.push_back(cellColor);
            m_normals.push_back(triangleNormal);

            /*
             * Pushing the information associated with the centroid to the
             * buffers.
             */
            m_positions.push_back(centroid);
            m_colors.push_back(cellColor);
            m_normals.push_back(triangleNormal);

            previous = current;
            current++;
        }

        /*
         * The last triangle between the last and first vertices, and the c
         * entroid has to be created "manually", after the loop.
         */
        p1.x = (*listIt)->front().first;
        p1.y = (*listIt)->front().second;
        p1.z = 0.0;
        
        glm::vec3 triangleNormal(
            glm::normalize(glm::cross(p1-p2, centroid-p2))    
        );

        m_positions.push_back(p2);
        m_colors.push_back(cellColor);
        m_normals.push_back(triangleNormal);

        m_positions.push_back(p1);
        m_colors.push_back(cellColor);
        m_normals.push_back(triangleNormal);

        m_positions.push_back(centroid);
        m_colors.push_back(cellColor);
        m_normals.push_back(triangleNormal);
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
    glGenBuffers(1, &m_colorsBufferID);
    glGenBuffers(1, &m_normalsBufferID);

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
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_colorsBufferID));
    glcheck(glBufferData(
                GL_ARRAY_BUFFER, 
                m_colors.size()*sizeof(glm::vec4), 
                m_colors.data(), 
                GL_STATIC_DRAW
            )
    );
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_normalsBufferID));
    glcheck(glBufferData(
                GL_ARRAY_BUFFER, 
                m_normals.size()*sizeof(glm::vec3), 
                m_normals.data(), 
                GL_STATIC_DRAW
            )
    );

}

Map2DRenderable::~Map2DRenderable()
{
    glcheck(glDeleteBuffers(1, &m_positionsBufferID));
    glcheck(glDeleteBuffers(1, &m_colorsBufferID));
    glcheck(glDeleteBuffers(1, &m_normalsBufferID));
}

void Map2DRenderable::do_draw()
{
    /*
     * Drawing geometric data
     */
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

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

    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_colorsBufferID));
        glcheck(glVertexAttribPointer(
                    colorLocation, 
                    4, 
                    GL_FLOAT, 
                    GL_FALSE, 
                    0, 
                    (void*)0
                )
        );
    }

    if(normalLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_normalsBufferID));
        glcheck(glVertexAttribPointer(
                    normalLocation, 
                    3, 
                    GL_FLOAT, 
                    GL_FALSE, 
                    0, 
                    (void*)0
                )
        );
    }

    /*
     * Drawing triangles elements.
     */
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    if(positionLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if(colorLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
    if(normalLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
}

void Map2DRenderable::do_animate(float time) 
{

}

void Map2DRenderable::insertIntoList(
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

glm::vec4 Map2DRenderable::biomeColor(Biome biome)
{
    switch (biome)
	{
        case Sea:
            return glm::vec4(0.00f, 0.345f, 1.00f, 1.00f);
            break;

        case Lake:
            return glm::vec4(0.051f, 0.878f, 1.00f, 1.00f);
            break;

        case Beach:
            return glm::vec4(1.00f, 0.835f, 0.482f, 1.00f);
            break;

        case Plains:
            return glm::vec4(0.165f, 0.910f, 0.220f, 1.00f);
            break;

        case Mountain:
            return glm::vec4(0.510f, 0.157f, 0.051f, 1.00f);
            break;

	case Peak:
            return glm::vec4(0.910f, 0.157f, 0.051f, 1.00f);
            break;

        default:
            return glm::vec4(1.00f, 0.051f, 0.510f, 1.00f);
            break;
	}
}
