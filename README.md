# Ecosystem Generator

![alt text](https://ensiwiki.ensimag.fr/images/5/56/Shutter_Island_Skybox.png "A wolf hunting a bunch of rabbits on the Island")

This project was created as an end of semester project in the engineering school Ensimag (Grenoble, France).  
It consists of a ecosystem generator we can divide in two parts, namely the generation of the terrain and the behavior of the animals :
* Generation of the terrain is a procedural generation of an island with mountains, plains and sands using Voronoi diagram, tessellation and other technics;
* Animals are ruled with boids behavior and a graph to go from one behavior to another.

## Authors
* HAMMEN Maxence ([github](https://github.com/bluerock57))
* KACHER Ilyes ([github](https://github.com/IlyesKacher))
* LY Mickaël ([github](https://github.com/astcort))
* STOFFEL Mathieu ([github](https://github.com/matsKNY))

## Installation
### Linux
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

### Windows
**1)** Build the **_voro++_** library:
* Run CMake so as to generate the Visual Studio solution;
```bash
# Building voro++
cd lib/voro++
mkdir build
cd build
cmake ..
```
* Open the Visual Studio solution with the latter, and build both the "Debug" and "Release" versions.

**2)** Build the **_tinyobjloader_** library:
* Run CMake so as to generate the Visual Studio solution;
```bash
# Building tinyobjloader
cd extlib/Win32/tinyobjloader
mkdir build
cd build
cmake ..
```
* Open the Visual Studio solution with the latter, and build both the "Debug" and "Release" versions.

**3)** Build the **_FreeType_** library:
* Open the Visual Studio solution in the repository **_extlib/freetype-2.5.5/builds/windows/vc2010/_**, and build both the "Debug" and "Release" versions.

**4)** Build the project:
* Running CMake so as to generate the Visual Studio solutions (one in "Debug" mode and one in "Release" mode. This separation is entailed by the fact that different versions of the libraries are needed, according to the mode. It must exists a way to unify both thanks to CMake, but, so far, we have not found it);
```bash
# Building the project in Debug and Release modes
rm -fr build
mkdir build
cd build
mkdir BuildTree_Debug
cd BuildTree_Debug
cmake ../..
cd ..
mkdir BuildTree_Release
cd BuildTree_Release
cmake -D CMAKE_BUILD_TYPE=RELEASE ../..
```
* Open the Visual Studio solutions with the latter, and build them both (the Debug one in Debug mode and the Release one in Release mode);
* Copy the following **_dlls_** in the folder **_build_** created during the step **4)**:
    * _extlib/Win32/sfml-2.3.2/bin/sfml-graphics-2.dll_;
    * _extlib/Win32/sfml-2.3.2/bin/sfml-graphics-d-2.dll_;
    * _extlib/Win32/sfml-2.3.2/bin/sfml-system-2.dll_;
    * _extlib/Win32/sfml-2.3.2/bin/sfml-system-d-2.dll_;
    * _extlib/Win32/sfml-2.3.2/bin/sfml-window-2.dll_;
    * _extlib/Win32/sfml-2.3.2/bin/sfml-window-d-2.dll_;
    * _extlib/Win32/glew-1.13.0/bin/Release/Win32/glew32.dll_.
* Copy the wanted executable (namely **_main.exe_**, either in **_BuildTree_Debug/Debug_** or in **_BuildTree_Release/Release_**, according to the chosen compilation mode) in the folder **_build_** created during the step **4)**.

**5)** Execute the project by running **_main.exe_**. 

## Dependencies

* **CMake** : CMake is an open-source, cross-platform family of tools designed to build, test and package software. For more info, click on [this link](https://cmake.org/);
* **gcc** : gcc is a compiler collection for C++ and other languages. For more info, click on [this link](https://gcc.gnu.org/);
* **Visual Studio 2015**: Visual Studio 2015 is a complete Development Environment (including, notably, an IDE and a compiler), created and developped by Microsoft. For more info, click on [this link](https://www.microsoft.com/france/visual-studio/essayez/telecharger/visual-studio.aspx);
* **Doxygen** : Doxygen is a standard tool for generating documentation. For more info, click on [this link](http://www.stack.nl/~dimitri/doxygen/index.html);
* **Rapidjson** : Rapidjson is a fast JSON parser/generator for C++ with both SAX/DOM style API. For more info, click on [this link](https://github.com/miloyip/rapidjson);
* **Voro++** : Voro++ is a software library for carrying out three-dimensional computations of the Voronoi tessellation. For more info, click on [this link](http://math.lbl.gov/voro++/);
* **C++ Bitmap Library** : The C++ Bitmap Library consists of simple, robust, optimized and portable processing routines for the 24-bit per pixel bitmap image format. For more info, click on [this link](http://partow.net/programming/bitmap/index.html).

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

* On rare occasion, an exception occur following a bad management of the boid when it moves from one cell to an other in the grid (06/08/2016);
* If a boid gets too close to the border of the map and depending on the precision of the grid, an exception will occur (06/08/2016);
* The boid can have unexpected behavior when walking on the mountains.

### Terrain issues (last update : 06/22/2016)

* Sometimes, a small wall can cut the map in two. This is due to the interpolation extending the influence of the mountain on a horizontal/vertical line.

## Acknowledgements

A very warm thank you to our two awesome computer graphics teachers who helped us during this project : 
* DELAME Thomas ([website](http://virtual-atom.com/))
* MANTEAUX Pierre-Luc ([github](http://manteapi.github.io/))

## License

See LICENSE.

## Bibliography

See BIBLIOGRAPHY.md.


## Credits

### Meshes

The following meshes have been used :
* [Rabbit](http://www.blendswap.com/blends/view/81060)
* [Wolf](http://tf3dm.com/3d-model/wolf-rigged-and-game-ready-42808.html)
* [Tree](http://www.blendswap.com/blends/view/71743)

The 'carrot' has been created by Ilyes Kacher.

