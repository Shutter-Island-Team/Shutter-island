#include "../include/Viewer.hpp"
#include "../include/log.hpp"

#include <sstream>

#include "../include/initialize_scene.hpp"

/**
 * @brief
 * Defines the size of the map (constrained square plane).
 */
#define MAP_SIZE 500.0

int main( int argc, char* argv[] )
{
    Viewer viewer(1280,720);

     /*
      * Creating the map generator and generating the map.
      */
    MapGenerator mapGenerator(MAP_SIZE);
    mapGenerator.compute();

    //initialize_map2D(viewer, mapGenerator, MAP_SIZE);
    initialize_test_scene(viewer, mapGenerator, MAP_SIZE);

    while( viewer.isRunning() )
    {
    	viewer.handleEvent();
    	viewer.animate();
    	viewer.draw();
    	viewer.display();
    }

    return EXIT_SUCCESS;
}
