# Ecosystem generator

This project was created BLA

## Author
* HAMMEN Maxence ([github profile link](https://github.com/bluerock57))
* KACHER Ilyes ([github profile link](https://github.com/IlyesKacher))
* LY Mickaël ([github profile link](https://github.com/astcort))
* STOFFEL Mathieu

## Installation

```bash
#Compile external libraries (if new lib has been added)
cd extlib/
make clean_all
make

#Compile project
cd ../
rm -rf build/
mkdir build/
cd build/
cmake ..
make -j 8
```

## Dependencies

* **CMake** : CMake is an open-source, cross-platform family of tools designed to build, test and package software. For more info, click on [this link](https://cmake.org/)
* **gcc** : gcc is a compiler collection for C++ and other languages. For more info, click on [this link](https://gcc.gnu.org/). 
* **Doxygen** : Doxygen is a standard tool for generating documentation. For more info, click on [this link](http://www.stack.nl/~dimitri/doxygen/index.html).
* **Rapidjson** : Rapidjson is a fast JSON parser/generator for C++ with both SAX/DOM style API. For more info, clik on [this link](https://github.com/miloyip/rapidjson).
* **Voro++** : Voro++ is a software library for carrying out three-dimensional computations of the Voronoi tessellation. For more info, click on [this link](http://math.lbl.gov/voro++/)

## Known issues
### Boid issues (last update : 06/08/2016)

* On rare occasion, an exception occur following a bad management of the boid when it moves from one cell to an other in the grid (06/08/2016)
* If a boid gets too close to the border of the map and depending on the precision of the grid, an exception will occur (06/08/2016)

### Terrain issues (last update : 06/08/2016)

No known bugs to this date

## Credits

A very warm thank you to our two awesome teachers who helped us (to be continued... BLA) : 
* DELAME Thomas
* MANTEAUX Pierre-Luc

## License

TODO: Write license
