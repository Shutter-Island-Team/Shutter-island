#include "../include/Viewer.hpp"
#include "../include/log.hpp"

#include <sstream>

#include "../include/practical_07_scene.hpp"
#include "../include/terrain/MapGenerator.hpp"

int main( int argc, char* argv[] )
{
    
    MapGenerator map(500);

    map.compute();

    for (int i = 0; i <= 500; i++)
	for (int j = 0; j <= 500; j++)
	    std::cout << i << "," << j << "," << map.getHeight(i,j) << std::endl;

    return EXIT_SUCCESS;
}
