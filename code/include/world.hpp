#ifndef WORLD_HPP
#define WORLD_HPP

#include "Viewer.hpp"
#include "./dynamics/DynamicSystem.hpp"
#include "./dynamics/DynamicSystemRenderable.hpp"


// This class builds the world (floor and walls) of the game
// To add other models to the world, the functions should have as prototype
//
// void thing_to_add(Viewer& viewer, DynamicSystemPtr& system, 
//                   DynamicSystemRenderablePtr &systemRenderable)
//
// And be called here with the world's variable
// (done as in the pratical 5)


void initialize_world(Viewer& viewer);

void initialize_world_box(Viewer& viewer, DynamicSystemPtr& system, 
			  DynamicSystemRenderablePtr &systemRenderable);

void initialize_world_light(Viewer& viewer, DynamicSystemPtr& system, 
			    DynamicSystemRenderablePtr &systemRenderable);


#endif
