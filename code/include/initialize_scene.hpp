#ifndef INITIALIZE_SCENE_HPP
#define INITIALIZE_SCENE_HPP

#include "Viewer.hpp"

void initialize_boid_scene_multiple_pop( Viewer& viewer );
void initialize_boid_scene_test_separate( Viewer& viewer );
void initialize_boid_scene_test_canSee( Viewer& viewer );
void initialize_boid_scene_test_machine_state( Viewer& viewer );
void test_create_MovableParameters_from_file();
void initialize_test_sight( Viewer& viewer);
void initialize_boid_scene_debug( Viewer& viewer);
void initialize_boid_scene_follow_leader( Viewer& viewer );
void initialize_boid_scene_hunt( Viewer& viewer );
void initialize_map2D(Viewer& viewer);

#endif 
