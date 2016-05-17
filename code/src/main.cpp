#include "../include/Viewer.hpp"
#include "../include/log.hpp"

#include <sstream>

void initialize_practical_05_scene( Viewer& viewer );


int main( int argc, char* argv[] )
{
    Viewer viewer(1280,720);

    initialize_practical_05_scene( viewer );
    
    while( viewer.isRunning() )
    {
	viewer.handleEvent();
	viewer.draw();
	viewer.animate();
	viewer.display();
    }

    return EXIT_SUCCESS;
}
