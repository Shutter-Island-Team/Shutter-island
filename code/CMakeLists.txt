#==========================================
#Project options.
#==========================================
cmake_minimum_required(VERSION 2.8)

#==========================================
#Project name.
#==========================================
project(TheIsland)

#==========================================
#Building options.
#==========================================
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -fopenmp -Wno-deprecated")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g -gdwarf-2")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp /D_CRT_SECURE_NO_DEPRECATE /D_USE_MATH_DEFINES")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DDEBUG")
endif()

#==========================================
#Building mode handling.
#==========================================
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG)
    set(CMAKE_CONFIGURATION_TYPES Debug)
else()
    set(CMAKE_CONFIGURATION_TYPES Release)
endif()
message("Building mode: " ${CMAKE_BUILD_TYPE})

#==========================================
#Libraries path : glm, sfml, glew, opengl, freetype and voro++.
#==========================================

#GLM Libraries
set(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/glm-0.9.7.1" CACHE PATH "glm")
include_directories(${GLM_INCLUDE_DIRS})

#GLEW Libraries
if(UNIX)
    set(GLEW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Linux/glew-1.13.0/include" CACHE PATH "glew")
    include_directories(${GLEW_INCLUDE_DIRS})
    set(GLEW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Linux/glew-1.13.0/lib/libGLEW.so" CACHE FILEPATH "glew")
elseif(APPLE)
    message("APPLE platform not handled")
elseif(WIN32)
    set(GLEW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/glew-1.13.0/include" CACHE PATH "glew")
    include_directories(${GLEW_INCLUDE_DIRS})
    set(GLEW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/glew-1.13.0/lib/Release/Win32/glew32.lib" CACHE FILEPATH "glew")
endif()

#SFML Libraries
if(UNIX)
    set(SFML_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Linux/sfml-2.3.2/include/" CACHE PATH "sfml")
    include_directories(${SFML_INCLUDE_DIRS})
    set(SFML_SYSTEM_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Linux/sfml-2.3.2/build/lib/libsfml-system.so" CACHE FILEPATH "sfml")
    set(SFML_WINDOW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Linux/sfml-2.3.2/build/lib/libsfml-window.so" CACHE FILEPATH "sfml")
    set(SFML_GRAPHICS_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Linux/sfml-2.3.2/build/lib/libsfml-graphics.so" CACHE FILEPATH "sfml")
elseif(APPLE)
    message("APPLE platform not handled")
elseif(WIN32)
    set(SFML_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/sfml-2.3.2/include/" CACHE PATH "sfml")
    include_directories(${SFML_INCLUDE_DIRS})
    if (CMAKE_BUILD_TYPE MATCHES RELEASE)
        set(SFML_SYSTEM_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/sfml-2.3.2/lib/sfml-system.lib" CACHE FILEPATH "sfml")
        set(SFML_WINDOW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/sfml-2.3.2/lib/sfml-window.lib" CACHE FILEPATH "sfml")
        set(SFML_GRAPHICS_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/sfml-2.3.2/lib/sfml-graphics.lib" CACHE FILEPATH "sfml")
    elseif (CMAKE_BUILD_TYPE MATCHES DEBUG)
        set(SFML_SYSTEM_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/sfml-2.3.2/lib/sfml-system-d.lib" CACHE FILEPATH "sfml")
        set(SFML_WINDOW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/sfml-2.3.2/lib/sfml-window-d.lib" CACHE FILEPATH "sfml")
        set(SFML_GRAPHICS_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/sfml-2.3.2/lib/sfml-graphics-d.lib" CACHE FILEPATH "sfml")
    endif()
endif()

#FREETYPE Libraries
set(FREETYPE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/freetype-2.5.5/include" CACHE PATH "freetype")
include_directories(${FREETYPE_INCLUDE_DIRS})
if(UNIX)
    set(FREETYPE_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/freetype-2.5.5/build/libfreetype.a" CACHE FILEPATH "freetype")
elseif(APPLE)
    message("APPLE platform not handled")
elseif(WIN32)
    if (CMAKE_BUILD_TYPE MATCHES RELEASE)
        set(FREETYPE_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/freetype-2.5.5/objs/vc2010/Win32/freetype255.lib" CACHE FILEPATH "freetype")
    elseif (CMAKE_BUILD_TYPE MATCHES DEBUG)
        set(FREETYPE_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/freetype-2.5.5/objs/vc2010/Win32/freetype255d.lib" CACHE FILEPATH "freetype")
    endif()
endif()

#TINYOBJLOADER Libraries
if(UNIX)
    set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Linux/tinyobjloader/" CACHE PATH "tinyobjloader")
    include_directories(${TINYOBJLOADER_INCLUDE_DIRS})
set(TINYOBJLOADER_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Linux/tinyobjloader/build/libtinyobjloader.a" CACHE FILEPATH "tinyobjloader")
elseif(APPLE)
    message("APPLE platform not handled")
elseif(WIN32)
    set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/tinyobjloader/" CACHE PATH "tinyobjloader")
    include_directories(${TINYOBJLOADER_INCLUDE_DIRS})
    if (CMAKE_BUILD_TYPE MATCHES RELEASE)
        set(TINYOBJLOADER_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/tinyobjloader/build/Release/tinyobjloader.lib" CACHE FILEPATH "tinyobjloader")
    elseif (CMAKE_BUILD_TYPE MATCHES DEBUG)
        set(TINYOBJLOADER_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/extlib/Win32/tinyobjloader/build/Debug/tinyobjloader.lib" CACHE FILEPATH "tinyobjloader")
    endif()
endif()

#Voro++ Libraries
set(VOROPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/lib/voro++/" CACHE PATH "voro")
include_directories(${VOROPP_INCLUDE_DIRS})
if(UNIX)
    set(VOROPP_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/voro++/src/libvoro++.a" CACHE FILEPATH "voro")
elseif(APPLE)
    message("Compile different.")
elseif(WIN32)
    if (CMAKE_BUILD_TYPE MATCHES RELEASE)
        set(VOROPP_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/voro++/src/Release/voro++.lib" CACHE FILEPATH "voro")
    elseif (CMAKE_BUILD_TYPE MATCHES DEBUG)
        set(VOROPP_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/voro++/src/Debug/voro++.lib" CACHE FILEPATH "voro")
    endif()
endif()

find_package(OpenGL REQUIRED)

#==============================================
#Project sources : src, include, shader, exe
#==============================================
file(
    GLOB_RECURSE
    HEADER_FILES
    include/*.hpp
    )

file(
    GLOB_RECURSE
    VORO_FILES
    lib/voro++/src/*.hpp
    )

file(
    GLOB_RECURSE
    BITMAP_FILES
    lib/bitmap/*.hpp
    )

file(
    GLOB_RECURSE
    SOURCE_FILES
    src/*.cpp
    )
    
file(
    GLOB_RECURSE
    SHADER_FILES
    shaders/*.glsl
    )

set(EXECUTABLE_NAME main)

#==============================================
#Project executable definition
#==============================================
add_executable(${EXECUTABLE_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES} ${VORO_FILES} ${BITMAP_FILES})

#==============================================
#Linking with libraries
#==============================================
if (OPENGL_FOUND)
    target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
    if (UNIX)
        target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
    endif()
endif()

target_link_libraries(${EXECUTABLE_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${FREETYPE_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_SYSTEM_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_WINDOW_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_GRAPHICS_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${TINYOBJLOADER_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${VOROPP_LIBRARIES})
 
