#ifndef VORONOI_SEEDS_GENERATOR_HPP
#define VORONOI_SEEDS_GENERATOR_HPP

/**@file
 * @brief Define a pseudo-random seeds generator for Voronoi diagram's verteces.
 *
 * This generator will produce a given number of pair of coordinates, which
 * are going to represent the Voronoi diagram's verteces.
 */

#include "Seed.hpp"
#include <list>
#include <vector>

/*
 * Defining the default parameters of the generator.
 */
#define VSG_WIDTH 				500.0
#define VSG_HEIGHT 				500.0
#define VSG_NB_SEEDS 			250
#define VSG_WIDTH_SUB 			10
#define VSG_HEIGHT_SUB 			10
#define VSG_MAX_SEEDS_BY_SUB 	5
#define VSG_MIN_DIST            10.0

/*
 * Defining a useful macro to compute the distance between two points.
 */
#define DISTANCE(x,y) sqrt(     ((x).getX()-(y).getX())*((x).getX()-(y).getX()) \
                            +   ((x).getY()-(y).getY())*((x).getY()-(y).getY()))

/*
 * Defining a useful macro to compute the "distance" from a point to the
 * center.
 * As a matter of fact, we just need this macro to sort the seeds according
 * to their distances to the center, so we just need an "image of the distance".
 * That is why we do not have to apply the square root, reducing the complexity
 * cost.
 */
#define DIST_TO_CENTER(x) ((x).getX()-m_width/2.0)*((x).getX()-m_width/2.0) \
                      +   ((x).getY()-m_height/2.0)*((x).getY()-m_height/2.0)

/**
 * @brief VoronoiSeedsGenerator interface.
 *
 * The generator produces pairs of coordinates which represent 2D points, namely
 * the vertices of a pseudo-random Voronoi diagram.
 *
 * The main aim of this generator is to allow us to produce "randomly" a
 * distribution of Voronoi vertices so as to create a "random" map, for the
 * Ecosystem we want to create, from the Voronoi diagram (each Voronoi cell is
 * going to be a "Biom" of the former).
 *
 * Quite obviously, purely "randomized" seeds may entail degenerated patterns
 * (from the Voronoi diagram's point of view) and, thus, degenerated maps.
 * That is why we decided to subdivide the considered 2D space thanks to a grid,
 * and to impose both a maximum number of generated seeds by subdivision and
 * a minimum distance between two generated seeds.
 * This is the first countermeasure against "degenerated patterns", the second
 * one being the use of Lloyd algorithm as a post-processing treatment of the
 * Voronoi diagram's generation.
 *
 * ####### TODO => Mettre un lien vers l'en-tête de la classe implémentant Lloyd
 *
 * Last piece of information to add about this generator : the generated
 * seeds are sorted according to their distance to the center of the map.
 * This is a pre-treatment of the "Whittaker step", so as to ease the latter
 * while reducing at most the complexity cost of sorting the seeds.
 */

 class VoronoiSeedsGenerator
 {
private:
	/**
	 * @brief The width of the considered 2D-space.
	 */
	float m_width;

	/**
	 * @brief The height of the considered 2D-space.
	 */
	float m_height;

	/**
	 * @brief The number of seeds to generate.
	 */
	int m_nbOfSeeds;

	/**
	 * @brief The number of subdivisions of the horizontal grid.
	 */
	int m_nbOfWidthSubdivisions;

	/**
	 * @brief The number of subdivisions of the vertical grid.
	 */
	int m_nbOfHeightSubdivisions;

	/**
	 * @brief The maximal number of seeds by subdivision of the global grid.
	 */
	int m_maxNbOfSeedsBySubdivision;

	/**
	 * @brief The minimal distance between two seeds.
	 */
	float m_minDist;

public:
	/**
	 * @brief Default constructor.
	 *
	 * Default values are used to set the parameters of the generator.
	 */
	VoronoiSeedsGenerator();

	/**
	 * @brief Full-parametrization constructor.
	 *
	 * Requires the user to provide all the needed parameters of the generator.
	 */
	VoronoiSeedsGenerator(
		float width,
		float height,
		int nbOfSeeds,
		int nbOfWidthSubdivisions,
		int nbOfHeightSubdivisions,
		int maxNbOfSeedsBySubdivision,
		float minDist
	);

	/**
	 * @brief Default destructor.
	 */
	~VoronoiSeedsGenerator();

	/**
	 * @brief Computes the pseudo-random seeds of Voronoi diagram.
	 *
	 * @param listOfSeeds A reference on the vector to fill with the randomly
	 *	generated seeds.
	 */
	void generateSeeds(std::vector<Seed>& listOfSeeds);

private:
    /**
     * @brief Performs a sorted insert of a seed into a list according to
     *  its distance to center of the map.
     *
     * @param list A reference of the list in which the insertion has to
     *  be performed.
     * @param seed The seed to insert into the list.
     */
    void insertIntoList(std::list<Seed>& list, Seed& seed);

    /**
     * @brief Verifies if the new randomly generated seed is sufficiently
     *  far from the previously inserted seeds.
     *
     * @param seedsOfSub A "pointer" on a bi-dimensional array representing
     *  the grid and containing a vector gathering the generated seeds,
     *  by cell.
     * @param widthID The column of the cell the newly generated seed should be
     *  inserted into.
     * @param heightID The line of the cell the newly generated seed should be
     *  inserted into.
     * @param seed The newly generated seed.
     */
    bool isMinDistVerified(
        std::vector<Seed> **seedsOfSub, 
        int widthID,
        int heightID,
        Seed seed
    );
 };

#endif // VORONOI_SEEDS_GENERATOR_HPP
