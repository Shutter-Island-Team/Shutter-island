#include "../include/Viewer.hpp"
#include "../include/log.hpp"

#include <sstream>

#include "../include/initialize_scene.hpp"

int main( int argc, char* argv[] )
{
    Viewer viewer(1280,720);

    //initialize_map2D(viewer);

    //initialize_boid_scene_multiple_pop( viewer );
    //initialize_boid_scene_test_separate( viewer );
    //initialize_boid_scene_test_canSee( viewer );
    //initialize_boid_scene_test_machine_state( viewer );
    //initialize_test_sight( viewer);
    //initialize_boid_scene_debug( viewer);
    //initialize_boid_scene_follow_leader( viewer );
    initialize_boid_scene_hunt( viewer );

    while( viewer.isRunning() )
    {
    	viewer.handleEvent();
    	viewer.animate();
    	viewer.draw();
    	viewer.display();
    }

    return EXIT_SUCCESS;
}
