#ifndef PRACTICAL_05_SCENE_HPP
#define PRACTICAL_05_SCENE_HPP

#include "Viewer.hpp"
#include "./dynamics/DynamicSystem.hpp"
#include "./dynamics/DynamicSystemRenderable.hpp"

void initialize_practical_05_scene( Viewer& viewer );
void practical05_particles(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable);
void practical05_springs(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable);
void practical05_collisions(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable);
void practical05_playPool(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable);

#endif 
