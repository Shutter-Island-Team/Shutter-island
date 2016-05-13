#ifndef KART_HPP
#define KART_HPP

#include "Viewer.hpp"
#include "./dynamics/DynamicSystem.hpp"
#include "./dynamics/DynamicSystemRenderable.hpp"



// Debug only
void add_Kart(Viewer& viewer, DynamicSystemPtr& system, 
	      DynamicSystemRenderablePtr &systemRenderable);


#endif 
