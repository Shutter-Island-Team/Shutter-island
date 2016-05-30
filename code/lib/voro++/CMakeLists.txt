# CMake (minimal) required version.
cmake_minimum_required(VERSION 2.8)

# Compilation flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_DEPRECATE")

# Header files location.
include_directories(
   "${CMAKE_CURRENT_SOURCE_DIR}/src"   
)

# Source files location.
file(
    GLOB_RECURSE
    host_sources    
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

# Destination directory of the generated static library.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Directive aiming at creating the static library.
ADD_LIBRARY( voro++ STATIC ${host_sources} )

#-------------------------------------------------------------------------------
# Compiling the static library.
TARGET_LINK_LIBRARIES( voro++ )

