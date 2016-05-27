/**
 * @file MapGenerator.cpp
 *
 * @see MapGenerator.hpp
 */


#define DEFAULT_NB_SEEDS            (300)      
#define DEFAULT_NB_SUBDIVISION      ( 25)
#define DEFAULT_NB_SEEDS_MAX_SUBDIV (  5)
#define DEFAULT_DIST_MIN            ( 10)


#include "../../include/terrain/Seed.hpp"
#include "../../include/terrain/MapGenerator.hpp"
#include "../../include/terrain/MapUtils.hpp"
#include "../../include/terrain/BiomeRepartition.hpp"
#include <iostream>

MapGenerator::MapGenerator(float size) :
    mapSize{size},
    voronoiSeedsGenerator{VoronoiSeedsGenerator(size, size,
						DEFAULT_NB_SEEDS,
						DEFAULT_NB_SUBDIVISION,
						DEFAULT_NB_SUBDIVISION,
						DEFAULT_NB_SEEDS_MAX_SUBDIV,
						DEFAULT_DIST_MIN)},
    seedsContainer(0.0, size+1, 0.0, size+1, -10.0, 10.0,
		   DEFAULT_NB_SUBDIVISION, DEFAULT_NB_SUBDIVISION, 1,
		   false, false, false,
		   1)
{}

MapGenerator::~MapGenerator() { 
    if (heightTree) {
        heightTree->freeHeightTree();
        heightTree = NULL;
    }
}
 

void MapGenerator::compute() {

    // Position Generation
    /*
     * Note that the seeds are ordered by their distance to the center of 
     * the map.
     * This will be useful for the biome repartition step.
     */
    voronoiSeedsGenerator.generateSeeds(seeds);

    // Voronoi step
    /*
     * Adding all the seeds to the container so as to generate Voronoi
     * diagram.
     * Since voro++ aims at computing tridimensional Voronoi diagrams, and
     * we only are interested in bidimensional ones, we decided to set the 
     * third dimension to "0.0", so that we may simulate a bidimensional
     * behavior of voro++.
     * In order to properly loop over the seeds during the "Whittaker Step",
     * we need to sort them according to the distance of the latter to a 
     * reference point (typically the middle of the map).
     * That is why we use a "particle_order" and a "loop_order" to retain
     * this order.
     */
    voro::particle_order seedsOrder;
    int ID = 0;
    for (
        auto iterator = seeds.begin(); 
        iterator != seeds.end();
        iterator++, ID++
    ) {
        seedsContainer.put(
            seedsOrder,
            ID,
            iterator->getX(),
            iterator->getY(),
            0.0
        );
    }

    voro::c_loop_order loopOrder(seedsContainer, seedsOrder);
    loopOrder.start();
    for (
        auto iterator = seeds.begin();
        iterator != seeds.end();
        iterator++, loopOrder.inc()
    ) {
        voroNeighborPtr currentCellPtr = std::make_shared<voro::voronoicell_neighbor>();
        seedsContainer.compute_cell(
            *currentCellPtr, 
            loopOrder
        );
        iterator->setCell(currentCellPtr);
        /*
         * Setting the centroids coordinates into the seed.
         */
        double x, y, z;
        currentCellPtr->centroid(x, y, z);
        iterator->setCentroidX(x + iterator->getX());
        iterator->setCentroidY(y + iterator->getY());
    }

    // Repartition land/sea
    computeLand(seeds, mapSize);

    // Computing the beaches depending on the seas
    computeBeach(seeds, mapSize);

    // Mountain repartition
    computeMountains(seeds);

    // Adding the lakes
    computeLake(seeds);

    // HeightTree step
    // Creating the initial map : a deep dark sea
    HeightData tlCorner(Vertex2D(0.0f,    mapSize), biomeHeight(Sea), Sea);
    HeightData trCorner(Vertex2D(mapSize, mapSize), biomeHeight(Sea), Sea);
    HeightData blCorner(Vertex2D(0.0f,    0.0f)   , biomeHeight(Sea), Sea);
    HeightData brCorner(Vertex2D(mapSize, 0.0f)   , biomeHeight(Sea), Sea);
    heightTree = new HeightTree(HeightNode(mapSize,
					   tlCorner, trCorner,
					   blCorner, brCorner));
    // Computing the tree
    heightTree->computeTree(seedsContainer, seeds);

}


Biome MapGenerator::getBiome(float x, float y) {
    Vertex2D position(x, y);
    return findClosestBiome(position, seedsContainer, seeds);
}

float MapGenerator::getHeight(float x, float y) {
    Vertex2D position(x, y);
    if (!heightTree) { 
        std::cerr << "HeightTree not computed !" << std::endl;
        exit(EXIT_FAILURE);
    }
    return heightTree->evalHeight(position);
}
