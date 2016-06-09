#include "../include/Viewer.hpp"
#include "../include/log.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <vector>

#include "../include/initialize_scene.hpp"
#include "../include/Utils.hpp"

int main( int argc, char* argv[] )
{
    std::srand(std::time(0));
    Viewer viewer(1280,720);

    /*
     * Parsing the JSon file containing the simulation parameters for the map.
     */
    MapParameters mapParameters("../mapData/MapParameters.json");

     /*
      * Creating the map generator and generating the map.
      */
    MapGenerator mapGenerator(mapParameters, mapParameters.getMapSize());
    mapGenerator.compute();

    //initialize_map2D(viewer, mapGenerator, MAP_SIZE);
    initialize_test_scene(viewer, mapGenerator, mapParameters.getMapSize());
    
    while( viewer.isRunning() )
    {
    	viewer.handleEvent();
    	viewer.animate();
    	viewer.draw();
    	viewer.display();
    }

    return EXIT_SUCCESS;
}
