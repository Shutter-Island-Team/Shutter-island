# Ecosystem generator

This project was created as a end of semester project in the university Ensimag (Grenoble, France). 

It consists of a ecosystem generator we can divid in two part : the generation of the terrain and the behavior of the animals :
* Generation of the terrain is a procedural generation of an island with mountains, plains and sands using Voronoi diagramm, tessellation and other technics.
* Animals are ruled with boids behavior and a graph to go from one behavior to an other.

## Author
* HAMMEN Maxence ([github](https://github.com/bluerock57))
* KACHER Ilyes ([github](https://github.com/IlyesKacher))
* LY Mickaël ([github](https://github.com/astcort))
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
* 

## Documentation

```bash
#Generate the documentation
cd doc
make

#To read the documentation
firefox html/index.html
```

## Known issues
### Boid issues (last update : 06/98/2016)

* On rare occasion, an exception occur following a bad management of the boid when it moves from one cell to an other in the grid (06/08/2016)
* If a boid gets too close to the border of the map and depending on the precision of the grid, an exception will occur (06/08/2016)
* The boid can have unexpected behavior when walking on the mountains (06/09/2016)

### Terrain issues (last update : 06/08/2016)

No known bugs to this date

## Credits

A very warm thank you to our two awesome teachers who helped us during the project : 
* DELAME Thomas ([website](http://virtual-atom.com/))
* MANTEAUX Pierre-Luc ([github](http://manteapi.github.io/))

## License

See LISENCE
