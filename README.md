# Ecosystem generator

This project was created as an end of semester project in the engineering school Ensimag (Grenoble, France). 

It consists of a ecosystem generator we can divide in two part : the generation of the terrain and the behavior of the animals :
* Generation of the terrain is a procedural generation of an island with mountains, plains and sands using Voronoi diagramm, tessellation and other technics.
* Animals are ruled with boids behavior and a graph to go from one behavior to an other.

## Authors
* HAMMEN Maxence ([github](https://github.com/bluerock57))
* KACHER Ilyes ([github](https://github.com/IlyesKacher))
* LY Mickaël ([github](https://github.com/astcort))
* STOFFEL Mathieu ([github](https://github.com/matsKNY))

## Installation

```bash
#Compile voro++
cd lib/voro++/src
make

#Compile external libraries
cd extlib/
make

#Compile project (In root of the project)
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
* **C++ Bitmap Library** : The C++ Bitmap Library consists of simple, robust, optimized and portable processing routines for the 24-bit per pixel bitmap image format. For more info, click on [this link](http://partow.net/programming/bitmap/index.html)

## Documentation

```bash
#Generate the documentation
cd doc
make

#To read the documentation
firefox html/index.html
```

## Known issues
### Boid issues (last update : 06/09/2016)

* On rare occasion, an exception occur following a bad management of the boid when it moves from one cell to an other in the grid (06/08/2016)
* If a boid gets too close to the border of the map and depending on the precision of the grid, an exception will occur (06/08/2016)
* The boid can have unexpected behavior when walking on the mountains

### Terrain issues (last update : 06/09/2016)

* Sometimes, a small wall can cut the map in two. This is due to the interpolation extending the influence of the mountain on a horizontal/vertical line.
* The computation of lake connexity is actually not correct. Because lakes are treated in a specific order, two lakes can sometimes not be connected if they are connected through another lake that is treated after these two.  

## Acknowledgements

A very warm thank to our two awesome teachers who helped us during the project : 
* DELAME Thomas ([website](http://virtual-atom.com/))
* MANTEAUX Pierre-Luc ([github](http://manteapi.github.io/))

## License

See LICENSE

## Bibliography

See BIBLIOGRAPHY.md


## Credits

### Meshes

The following meshes have been used :
* [Rabbit](http://www.blendswap.com/blends/view/81060)
* [Wolf](http://tf3dm.com/3d-model/wolf-rigged-and-game-ready-42808.html)
* [Tree](http://www.blendswap.com/blends/view/71743)

The 'carrot' has been created by Ilyes Kacher.