/**
 * @file MapGenerator.cpp
 *
 * @see MapGenerator.hpp
 */


#define DEFAULT_NB_SEEDS            (100)      
#define DEFAULT_NB_SUBDIVISION      ( 25)
#define DEFAULT_NB_SEEDS_MAX_SUBDIV (  5)
#define DEFAULT_DIST_MIN            ( 10)

#define PROB_MOUNTAIN (0.7)

#include "../../include/terrain/Seed.hpp"
#include "../../include/terrain/MapGenerator.hpp"
#include "../../include/terrain/BiomeRepartition.hpp"

MapGenerator::MapGenerator(float size) :
    mapSize{size},
    voronoiSeedsGenerator{VoronoiSeedsGenerator(size, size,
						DEFAULT_NB_SEEDS,
						DEFAULT_NB_SUBDIVISION,
						DEFAULT_NB_SUBDIVISION,
						DEFAULT_NB_SEEDS_MAX_SUBDIV,
						DEFAULT_DIST_MIN)},
    seedsContainer(0.0, size, 0.0, size, -10.0, 10.0,
		   DEFAULT_NB_SUBDIVISION, DEFAULT_NB_SUBDIVISION, 1,
		   false, false, false,
		   1)
{}

#include <iostream>
void MapGenerator::compute() {

    // Position Generation
    /*
     * Note that the seeds are ordered by their distance to the center of 
     * the map.
     * This will be useful for the "Whittaker Step".
     */
    std::vector<Seed> seeds;
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
    }

    // Repartition land/sea
    computeLand(seeds, mapSize);

    // Mountain repartition
    raiseMountains(seeds, PROB_MOUNTAIN);

    // HeightTree step

    // Gg wp


}


