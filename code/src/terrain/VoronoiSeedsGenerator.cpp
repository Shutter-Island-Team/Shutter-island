#include "../../include/terrain/Seed.hpp"
#include "../../include/terrain/VoronoiSeedsGenerator.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <random>
#include <vector>

VoronoiSeedsGenerator::VoronoiSeedsGenerator()
	:	m_width(VSG_WIDTH),
		m_height(VSG_HEIGHT),
		m_nbOfSeeds(VSG_NB_SEEDS),
		m_nbOfWidthSubdivisions(VSG_WIDTH_SUB),
		m_nbOfHeightSubdivisions(VSG_HEIGHT_SUB),
		m_maxNbOfSeedsBySubdivision(VSG_MAX_SEEDS_BY_SUB),
        m_minDist(VSG_MIN_DIST)
{

}

VoronoiSeedsGenerator::VoronoiSeedsGenerator(
	float width,
	float height,
	int nbOfSeeds,
	int nbOfWidthSubdivisions,
	int nbOfHeightSubdivisions,
	int maxNbOfSeedsBySubdivision,
    float minDist
)
	:	m_width(width),
		m_height(height),
		m_nbOfSeeds(nbOfSeeds),
		m_nbOfWidthSubdivisions(nbOfWidthSubdivisions),
		m_nbOfHeightSubdivisions(nbOfHeightSubdivisions),
		m_maxNbOfSeedsBySubdivision(maxNbOfSeedsBySubdivision),
        m_minDist(minDist)
{
	int effectiveMaxNbOfSeeds = 	nbOfWidthSubdivisions
								*	nbOfHeightSubdivisions
								*	maxNbOfSeedsBySubdivision;

	if (nbOfSeeds > effectiveMaxNbOfSeeds) {
        std::cerr << 	"There is too much seeds to generate, given the limiting parameters." << std::endl;
		exit(EXIT_FAILURE);
	}
}

VoronoiSeedsGenerator::~VoronoiSeedsGenerator()
{

}

void VoronoiSeedsGenerator::generateSeeds(
	std::vector<Seed>& listOfSeeds
)
{
	/*
	 * In order to retain the current number of seeds by subdivision of the
	 * grid, we use a bi-dimensional array.
	 */
	int nbSeedsBySub[m_nbOfHeightSubdivisions][m_nbOfWidthSubdivisions] = {};

    /*
     * So as to retain the repartition of the inserted seeds, we need to allocate another array.
     * This one is dynamically allocated since it will be necessary to use it as an argument
     * for a function.
     */
    std::vector<Seed>** seedsBySub = new std::vector<Seed>* [m_nbOfHeightSubdivisions];
    for (int i = 0; i < m_nbOfHeightSubdivisions; i++) {
        seedsBySub[i] = new std::vector<Seed> [m_nbOfWidthSubdivisions];
    }

	/*
	 * Initializing the random generator which is going to be used in order to
	 * generate the seeds.
	 */
	std::default_random_engine generator;
	std::normal_distribution<float> widthDistrib(m_width/2.0, m_width/5.0);
	std::normal_distribution<float> heightDistrib(m_height/2.0, m_height/5.0);

	/*
	 * Main loop of the function in which the seeds are generated.
     * On top of that, in order to ease the implementation of the "Whittaker
     * step", the seeds have to be sorted according to their distance to the
     * center of the map.
     * So as to do so, the seeds are sorted by "insertion" in a temporary list,
     * and then pushed back in the given vector.
	 */
    std::list<Seed> tmpList;
	int currentNbOfSeeds = 0;
	while (currentNbOfSeeds < m_nbOfSeeds) {
		float wPosition = widthDistrib(generator);
		float hPosition = heightDistrib(generator);

		/*
		 * Testing if the subdivision which is going to contain the new seed
		 * is "full".
		 */
		int widthID = (int)(floor(wPosition/m_width*m_nbOfWidthSubdivisions));
		int heightID = (int)(floor(hPosition/m_height*m_nbOfHeightSubdivisions));

        if (
                (widthID >= 0) && (widthID < m_nbOfWidthSubdivisions) 
            &&  (heightID >=0) && (heightID < m_nbOfHeightSubdivisions)
        ) {
            if (nbSeedsBySub[heightID][widthID] < m_maxNbOfSeedsBySubdivision) {
                Seed seed(wPosition, hPosition);

                /*
                 * Inserting only if the new seed is at a minimal distance 
                 * of the other ones previously inserted.
                 */
                if (isMinDistVerified(seedsBySub, widthID, heightID, seed)) {
                    insertIntoList(tmpList, seed);
                    currentNbOfSeeds++;
                    nbSeedsBySub[heightID][widthID]++;
                    seedsBySub[heightID][widthID].push_back(seed);
                }
            }
        }
	}

    /*
     * Freeing the allocated memory zone related to seeds' repartition.
     */
    for (int i = 0; i < m_nbOfHeightSubdivisions; i++) {
      seedsBySub[i]->clear();
        delete [] seedsBySub[i];
    }
    delete [] seedsBySub;

    /*
     * Finally, we just have to insert the content of the list into the
     * given vector.
     */
    for (
        auto iterator = tmpList.begin();
        iterator != tmpList.end();
        iterator++
    ) {
        listOfSeeds.push_back(*iterator);
    }
}

void VoronoiSeedsGenerator::insertIntoList(std::list<Seed>& list, Seed& seed)
{
    /*
     * If the list is empty, just pushing back the new seed.
     * Otherwise, we have to determine the position to insert the seed to.
     *  In order to do so, we simply iterate over the seeds, comparing the
     *  distance to center of the one to insert with the latter's ones.
     *  The last comparison needs extra care, since we may have to push
     *  back the seed to insert if its distance to center is greater than
     *  the one of the list's last seed.
     */
    if (list.empty()) {
        list.push_back(seed);
    } else {
        auto current = list.begin();
        auto last = list.end();
        float distance = DIST_TO_CENTER(seed);
        while ((current != last) && (distance > DIST_TO_CENTER(*current))) {
            current++;   
        }
        if (distance <= DIST_TO_CENTER(*current)) {
            list.insert(current, seed);
        } else {
            list.push_back(seed);
        }
    }
}
 
bool VoronoiSeedsGenerator::isMinDistVerified(
    std::vector<Seed> **seedsOfSub,
    int widthID,
    int heightID,
    Seed seed
)
{
    /*
     * This function is the most critical one in terms of algorithm complexity.
     * That is why, even if it is not really reader-friendly, all the subtests
     * which aim at verifying if the considered cells has neighbours have been
     * unfolded.
     * As a matter of fact, unfolding them allows us to avoid numerous - due to
     * the loops - tests.
     */
    if (heightID > 0) {
        if (widthID > 0) {
            /*
             * SW subdivision.
             */
            for (
                    auto iterator = seedsOfSub[heightID-1][widthID-1].begin(); 
                    iterator != seedsOfSub[heightID-1][widthID-1].end();
                    iterator++
                ) {
                if (DISTANCE(*iterator, seed) < m_minDist) {
                    return false;
                }
            }
        }

        /*
         * S subdivision.
         */
        for (
                auto iterator = seedsOfSub[heightID-1][widthID].begin(); 
                iterator != seedsOfSub[heightID-1][widthID].end();
                iterator++
            ) {
            if (DISTANCE(*iterator, seed) < m_minDist) {
                return false;
            }
        }

        if (widthID < (m_nbOfWidthSubdivisions - 1)) {
            /*
             * SE subdivision.
             */
            for (
                    auto iterator = seedsOfSub[heightID-1][widthID+1].begin(); 
                    iterator != seedsOfSub[heightID-1][widthID+1].end();
                    iterator++
                ) {
                if (DISTANCE(*iterator, seed) < m_minDist) {
                    return false;
                }
            }
        }
    }

    if (widthID > 0) {
        /*
         * W subdivision.
         */
        for (
                auto iterator = seedsOfSub[heightID][widthID-1].begin(); 
                iterator != seedsOfSub[heightID][widthID-1].end();
                iterator++
            ) {
            if (DISTANCE(*iterator, seed) < m_minDist) {
                return false;
            }
        }
    }

    /*
     * Central subdivision.
     */
    for (
            auto iterator = seedsOfSub[heightID][widthID].begin(); 
            iterator != seedsOfSub[heightID][widthID].end();
            iterator++
        ) {
        if (DISTANCE(*iterator, seed) < m_minDist) {
            return false;
        }
    }

    if (widthID < (m_nbOfWidthSubdivisions - 1)) {
        /*
         * E subdivision.
         */
        for (
                auto iterator = seedsOfSub[heightID][widthID+1].begin(); 
                iterator != seedsOfSub[heightID][widthID+1].end();
                iterator++
            ) {
            if (DISTANCE(*iterator, seed) < m_minDist) {
                return false;
            }
        }
    }

    if (heightID < (m_nbOfHeightSubdivisions - 1)) {
        if (widthID > 0) {
            /*
             * NW subdivision.
             */
            for (
                    auto iterator = seedsOfSub[heightID+1][widthID-1].begin(); 
                    iterator != seedsOfSub[heightID+1][widthID-1].end();
                    iterator++
                ) {
                if (DISTANCE(*iterator, seed) < m_minDist) {
                    return false;
                }
            }
        }

        /*
         * N subdivision.
         */
        for (
                auto iterator = seedsOfSub[heightID+1][widthID].begin(); 
                iterator != seedsOfSub[heightID+1][widthID].end();
                iterator++
            ) {
            if (DISTANCE(*iterator, seed) < m_minDist) {
                return false;
            }
        }

        if (widthID < (m_nbOfWidthSubdivisions - 1)) {
            /*
             * NE subdivision.
             */
            for (
                    auto iterator = seedsOfSub[heightID+1][widthID+1].begin(); 
                    iterator != seedsOfSub[heightID+1][widthID+1].end();
                    iterator++
                ) {
                if (DISTANCE(*iterator, seed) < m_minDist) {
                    return false;
                }
            }
        }
    }

    return true;
}

