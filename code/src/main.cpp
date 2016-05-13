#include "../include/Viewer.hpp"
#include "../include/log.hpp"

#include <sstream>

void initialize_practical_05_scene( Viewer& viewer );

void initialize_world(Viewer& viewer);

int main( int argc, char* argv[] )
{
    Viewer viewer(1280,720);

    //initialize_practical_05_scene( viewer );
    initialize_world(viewer);
    
    double currentDate;
    double scheduledRedrawDate = -1.0;
    double durationBetweenRedraw = 1./50.;

    while( viewer.isRunning() )
    {
	viewer.handleEvent();
	currentDate = ((double) std::clock()/CLOCKS_PER_SEC);
	//if (currentDate > scheduledRedrawDate) {
	if (true) {  
	    viewer.draw();
	    scheduledRedrawDate = currentDate + durationBetweenRedraw;
	    viewer.animate();
	    viewer.display();
	} 
    }

    return EXIT_SUCCESS;
}
