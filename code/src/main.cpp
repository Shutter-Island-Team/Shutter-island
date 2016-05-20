#include "../include/Viewer.hpp"
#include "../include/log.hpp"

#include <sstream>

#include "../include/practical_07_scene.hpp"

int main( int argc, char* argv[] )
{
    Viewer viewer(1280,720);

    initialize_boid_scene( viewer );
    // initialize_boid_scene_test_separate( viewer );
    // initialize_practical_07_scene( viewer );

    while( viewer.isRunning() )
    {
	viewer.handleEvent();
	viewer.animate();
	viewer.draw();
	viewer.display();
    }

    return EXIT_SUCCESS;
}
