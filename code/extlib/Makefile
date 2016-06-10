all: build_glew build_sfml build_freetype build_tinyobjloader

build_glew:
	cd glew-1.13.0/ && make -j 8

build_sfml:
	cd sfml-2.3.2/ ; mkdir build ; cd build/ && cmake ../ && make -j 8

build_freetype:
	cd freetype-2.5.5/ ; mkdir build ; cd build/ && cmake ../ && make -j 8

build_tinyobjloader:
	cd tinyobjloader/ ; mkdir build ; cd build/ && cmake ../ && make -j 8

clean_all: clean_glew clean_sfml clean_freetype clean_tinyobjloader

clean_glew: 
	cd glew-1.13.0/ && rm lib/*

clean_sfml:
	cd sfml-2.3.2/ && rm -r build/

clean_freetype:
	cd freetype-2.5.5/ && rm -r build/

clean_tinyobjloader:
	cd tinyobjloader && rm -rf build/
