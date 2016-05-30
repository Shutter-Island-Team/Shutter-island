#include "./../include/initialize_scene.hpp"

#include "../include/ShaderProgram.hpp"
#include "../include/graphicPrimitives/FrameRenderable.hpp"
#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"
#include "../include/texturing/MultiTexturedCubeRenderable.hpp"
#include "../include/texturing/MipMapCubeRenderable.hpp"
#include "../include/texturing/TexturedLightedMeshRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"

#include "../include/boids2D/Rabbit.hpp"
#include "../include/boids2D/Wolf.hpp"
#include "../include/boids2D/Carrot.hpp"
#include "../include/boids2D/Tree.hpp"
#include "../include/boids2D/BoidRenderable.hpp"
#include "../include/boids2D/BoidsManager.hpp"
#include "../include/boids2D/MovableParameters.hpp"
#include "../include/boids2D/SightRenderable.hpp"
#include "../include/boids2D/StateRenderable.hpp"

#include "../include/graphicPrimitives/QuadRenderable.hpp"
#include "../include/terrain/MapGenerator.hpp"
#include "../include/terrain/Map2DRenderable.hpp"

#include <cstdlib>
#include <ctime>

#include "../include/boids2D/DynamicSystemBoid.hpp"
#include "../include/boids2D/DynamicSystemBoidRenderable.hpp"

#include "../include/Utils.hpp"

void display_boid( Viewer& viewer, BoidsManagerPtr boidsManager, 
    DynamicSystemBoidRenderablePtr systemRenderable, ShaderProgramPtr texShader, ShaderProgramPtr flatShader )
{
    for(MovableBoidPtr m : boidsManager->getMovableBoids())
    {
        BoidRenderablePtr br = std::make_shared<BoidRenderable>(texShader, m);
        br->setMaterial(Material::Pearl());
        
        #ifdef DEBUG
            SightRenderablePtr sight = std::make_shared<SightRenderable>(flatShader, m);
            viewer.addRenderable(sight);

            StateRenderablePtr state = std::make_shared<StateRenderable>(flatShader, m);
            viewer.addRenderable(state);
        #endif
        
        HierarchicalRenderable::addChild( systemRenderable, br );
    }

    for(RootedBoidPtr m : boidsManager->getRootedBoids())
    {
        BoidRenderablePtr br = std::make_shared<BoidRenderable>(texShader, m);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }
}
/*
void initialize_boid_scene_multiple_pop( Viewer& viewer)    
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 50 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );

    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/flatVertex.vert", 
        "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    MaterialPtr pearl = Material::Pearl();

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    viewer.setDirectionalLight(directionalLight);

    //Textured shader
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/textureVertex.vert",
        "../shaders/textureFragment.frag"});
    viewer.addShaderProgram( texShader );

    std::string filename = "./../textures/grass_texture.png";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    texPlane->setParentTransform(glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(50.0,50.0,50.0)), glm::vec3(0.0, 0.0, -0.1)));
    texPlane->setMaterial(pearl);
    viewer.addRenderable(texPlane);
    
    BoidsManagerPtr boidsManager = std::make_shared<BoidsManager>();

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemBoidPtr system = std::make_shared<DynamicSystemBoid>();
    SolverBoidPtr solver = std::make_shared<SolverBoid>();
    system->setSolver(solver);
    system->setDt(0.01);
    system->setBoidsManager(boidsManager);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemBoidRenderablePtr systemRenderable = std::make_shared<DynamicSystemBoidRenderable>(system);

    MovableBoidPtr leaderRabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
    leaderRabbit->setNewLeader(leaderRabbit);

    for (int i = 0; i < 6; ++i) {
        MovableBoidPtr rabbitFellow = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
        rabbitFellow->setNewLeader(leaderRabbit);
    }

    MovableBoidPtr leaderWolf = boidsManager->addMovableBoid(WOLF, glm::vec3(random(-15, 15), random(-15, 15), 2));
    leaderWolf->setNewLeader(leaderWolf);
    for (int i = 0; i < 4; ++i) {
        MovableBoidPtr wolfFellow = boidsManager->addMovableBoid(WOLF, glm::vec3(random(-15, 15), random(-15, 15), 2));
        wolfFellow->setNewLeader(leaderWolf);
    }

    for (int i = 0; i < 10; ++i) {
        boidsManager->addRootedBoid(CARROT, glm::vec3(random(-15, 15), random(-15, 15), 2));
    }

    for (int i = 0; i < 10; ++i) {
        boidsManager->addRootedBoid(TREE, glm::vec3(random(-15, 15), random(-15, 15), 2));
    }

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}

void initialize_boid_scene_test_separate( Viewer& viewer )
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 50 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );

    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/flatVertex.vert", 
        "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    MaterialPtr pearl = Material::Pearl();

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    viewer.setDirectionalLight(directionalLight);

    //Textured shader
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/textureVertex.vert",
        "../shaders/textureFragment.frag"});
    viewer.addShaderProgram( texShader );

    std::string filename = "./../textures/grass_texture.png";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    texPlane->setParentTransform(glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(50.0,50.0,50.0)), glm::vec3(0.0, 0.0, -0.1)));
    texPlane->setMaterial(Material::Pearl());
    viewer.addRenderable(texPlane);

    BoidsManagerPtr boidsManager = std::make_shared<BoidsManager>();

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemBoidPtr system = std::make_shared<DynamicSystemBoid>();
    SolverBoidPtr solver = std::make_shared<SolverBoid>();
    system->setSolver(solver);
    system->setDt(0.01);
    system->setBoidsManager(boidsManager);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemBoidRenderablePtr systemRenderable = std::make_shared<DynamicSystemBoidRenderable>(system);

    MovableBoidPtr rabbitBoid;

    MovableBoidPtr leaderRabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
    leaderRabbit->setNewLeader(leaderRabbit);

    rabbitBoid = boidsManager->addMovableBoid(RABBIT, glm::vec3(-5, -5, 2), glm::vec3(1, 0, 0));
    rabbitBoid->setNewLeader(leaderRabbit);
    rabbitBoid = boidsManager->addMovableBoid(RABBIT, glm::vec3(0, -1, 2), glm::vec3(0, -1, 0));
    rabbitBoid->setNewLeader(leaderRabbit);

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}

void initialize_boid_scene_test_canSee( Viewer& viewer )
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 50 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );

    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/flatVertex.vert", 
        "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    MaterialPtr pearl = Material::Pearl();

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    glm::vec3 lightPosition(0.0,0.0,20.0);
    viewer.setDirectionalLight(directionalLight);

    //Textured shader
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/textureVertex.vert",
        "../shaders/textureFragment.frag"});
    viewer.addShaderProgram( texShader );

    std::string filename = "./../textures/grass_texture.png";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    texPlane->setParentTransform(glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(50.0,50.0,50.0)), glm::vec3(0.0, 0.0, -0.1)));
    texPlane->setMaterial(Material::Pearl());
    viewer.addRenderable(texPlane);

    BoidsManagerPtr boidsManager = std::make_shared<BoidsManager>();

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemBoidPtr system = std::make_shared<DynamicSystemBoid>();
    SolverBoidPtr solver = std::make_shared<SolverBoid>();
    system->setSolver(solver);
    system->setDt(0.01);
    system->setBoidsManager(boidsManager);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemBoidRenderablePtr systemRenderable = std::make_shared<DynamicSystemBoidRenderable>(system);

    MovableBoidPtr leaderRabbit;

    leaderRabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(-5, -5, 2), glm::vec3(1, 0, 0));
    leaderRabbit->setNewLeader(leaderRabbit);
    leaderRabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(0, -1, 2), glm::vec3(0, -1, 0));
    leaderRabbit->setNewLeader(leaderRabbit);

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}

void initialize_boid_scene_test_machine_state( Viewer& viewer )
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 50 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );

    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/flatVertex.vert", 
        "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    MaterialPtr pearl = Material::Pearl();

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    viewer.setDirectionalLight(directionalLight);

    //Textured shader
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/textureVertex.vert",
        "../shaders/textureFragment.frag"});
    viewer.addShaderProgram( texShader );

    std::string filename = "./../textures/grass_texture.png";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    texPlane->setParentTransform(glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(50.0,50.0,50.0)), glm::vec3(0.0, 0.0, -0.1)));
    texPlane->setMaterial(Material::Pearl());
    viewer.addRenderable(texPlane);

    BoidsManagerPtr boidsManager = std::make_shared<BoidsManager>();

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemBoidPtr system = std::make_shared<DynamicSystemBoid>();
    SolverBoidPtr solver = std::make_shared<SolverBoid>();
    system->setSolver(solver);
    system->setDt(0.01);
    system->setBoidsManager(boidsManager);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemBoidRenderablePtr systemRenderable = std::make_shared<DynamicSystemBoidRenderable>(system);


    MovableBoidPtr leaderRabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
    leaderRabbit->setNewLeader(leaderRabbit);

    MovableBoidPtr rabbitBoid;

    for (int i = 0; i < 100; ++i) {
        rabbitBoid = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
        rabbitBoid->setNewLeader(rabbitBoid);
    }

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}

void initialize_test_sight( Viewer& viewer )
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 50 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );

    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/flatVertex.vert", 
        "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    MaterialPtr pearl = Material::Pearl();

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    glm::vec3 lightPosition(0.0,0.0,20.0);
    viewer.setDirectionalLight(directionalLight);

    //Textured shader
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/textureVertex.vert",
        "../shaders/textureFragment.frag"});
    viewer.addShaderProgram( texShader );

    std::string filename = "./../textures/grass_texture.png";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    texPlane->setParentTransform(glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(50.0,50.0,50.0)), glm::vec3(0.0, 0.0, -0.1)));
    texPlane->setMaterial(pearl);
    viewer.addRenderable(texPlane);
    
    BoidsManagerPtr boidsManager = std::make_shared<BoidsManager>();

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemBoidPtr system = std::make_shared<DynamicSystemBoid>();
    SolverBoidPtr solver = std::make_shared<SolverBoid>();
    system->setSolver(solver);
    system->setDt(0.01);
    system->setBoidsManager(boidsManager);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemBoidRenderablePtr systemRenderable = std::make_shared<DynamicSystemBoidRenderable>(system);

    MovableBoidPtr leaderRabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
    leaderRabbit->setNewLeader(leaderRabbit);

    MovableBoidPtr rabbitBoid;

    for (int i = 0; i < 100; ++i) {
        rabbitBoid = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
        rabbitBoid->setNewLeader(rabbitBoid);
    }

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}

void initialize_boid_scene_debug( Viewer& viewer)
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 50 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );

    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/flatVertex.vert", 
        "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    MaterialPtr pearl = Material::Pearl();

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    glm::vec3 lightPosition(0.0,0.0,20.0);
    viewer.setDirectionalLight(directionalLight);

    //Textured shader
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/textureVertex.vert",
        "../shaders/textureFragment.frag"});
    viewer.addShaderProgram( texShader );

    std::string filename = "./../textures/grass_texture.png";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    texPlane->setParentTransform(glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(50.0,50.0,50.0)), glm::vec3(0.0, 0.0, -0.1)));
    texPlane->setMaterial(pearl);
    viewer.addRenderable(texPlane);
    
    BoidsManagerPtr boidsManager = std::make_shared<BoidsManager>();

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemBoidPtr system = std::make_shared<DynamicSystemBoid>();
    SolverBoidPtr solver = std::make_shared<SolverBoid>();
    system->setSolver(solver);
    system->setDt(0.01);
    system->setBoidsManager(boidsManager);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemBoidRenderablePtr systemRenderable = std::make_shared<DynamicSystemBoidRenderable>(system);

    MovableBoidPtr rabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(-5, -5, 2), glm::vec3(1, 0, 0));
    rabbit->setNewLeader(rabbit);

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}

void initialize_boid_scene_follow_leader( Viewer& viewer )
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 50 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );

    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/flatVertex.vert", 
        "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    MaterialPtr pearl = Material::Pearl();

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    viewer.setDirectionalLight(directionalLight);

    //Textured shader
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/textureVertex.vert",
        "../shaders/textureFragment.frag"});
    viewer.addShaderProgram( texShader );

    std::string filename = "./../textures/grass_texture.png";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    texPlane->setParentTransform(glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(50.0,50.0,50.0)), glm::vec3(0.0, 0.0, -0.1)));
    texPlane->setMaterial(Material::Pearl());
    viewer.addRenderable(texPlane);

    BoidsManagerPtr boidsManager = std::make_shared<BoidsManager>();

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemBoidPtr system = std::make_shared<DynamicSystemBoid>();
    SolverBoidPtr solver = std::make_shared<SolverBoid>();
    system->setSolver(solver);
    system->setDt(0.01);
    system->setBoidsManager(boidsManager);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemBoidRenderablePtr systemRenderable = std::make_shared<DynamicSystemBoidRenderable>(system);

    MovableBoidPtr leader = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
    leader->setNewLeader(leader);

    MovableBoidPtr r;
    for (int i = 0; i < 6; ++i) {
        r = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
        r->setNewLeader(leader);
    }

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}

void initialize_boid_scene_hunt( Viewer& viewer )
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 50 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );

    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/flatVertex.vert", 
        "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    MaterialPtr pearl = Material::Pearl();

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    viewer.setDirectionalLight(directionalLight);

    //Textured shader
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/textureVertex.vert",
        "../shaders/textureFragment.frag"});
    viewer.addShaderProgram( texShader );

    std::string filename = "./../textures/grass_texture.png";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    texPlane->setParentTransform(glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(50.0,50.0,50.0)), glm::vec3(0.0, 0.0, -0.1)));
    texPlane->setMaterial(Material::Pearl());
    viewer.addRenderable(texPlane);

    BoidsManagerPtr boidsManager = std::make_shared<BoidsManager>();

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemBoidPtr system = std::make_shared<DynamicSystemBoid>();
    SolverBoidPtr solver = std::make_shared<SolverBoid>();
    system->setSolver(solver);
    system->setDt(0.01);
    system->setBoidsManager(boidsManager);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemBoidRenderablePtr systemRenderable = std::make_shared<DynamicSystemBoidRenderable>(system);

    MovableBoidPtr wolf = boidsManager->addMovableBoid(WOLF, glm::vec3(random(-15, 15), random(-15, 15), 2));
    wolf->setNewLeader(wolf);
    MovableBoidPtr rabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
    MovableBoidPtr rabbit2 = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(-15, 15), random(-15, 15), 2));
    rabbit->setNewLeader(rabbit);
    rabbit2->setNewLeader(rabbit);

    wolf->setMovablePrey(rabbit);
    rabbit->setHunter(wolf);
    rabbit2->setHunter(wolf);

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}
*/
void initialize_map2D(Viewer& viewer, MapGenerator& mapGenerator, float mapSize)
{
    /*
     * Positionning the camera.
     */
    viewer.getCamera().setViewMatrix(glm::lookAt( 
                                        glm::vec3(mapSize/2.0, mapSize/2.0, 1.50*mapSize/2.0), 
                                        glm::vec3(mapSize/2.0, mapSize/2.0, 0), 
                                        glm::vec3( 0, 1, 0) 
                                    ) 
    );

    /*
     * Loading default shaders.
     */
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
            std::list<std::string>{
                "../shaders/flatVertex.vert", 
                "../shaders/flatFragment.frag"
            }
    );
    viewer.addShaderProgram(flatShader);

    /*
     * Creating the map renderable and adding it to the system.
     */
    Map2DRenderablePtr mapRenderable = std::make_shared<Map2DRenderable>(flatShader, mapGenerator);
    viewer.addRenderable(mapRenderable);

    /*
     * Creating a QuadRenderable at the altitude 0, so as to represent a calm sea.
     */
    PlaneRenderablePtr seaRenderable = std::make_shared<QuadRenderable>(
        flatShader,
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(mapSize, 0.0, 0.0),
        glm::vec3(mapSize, mapSize, 0.0),
        glm::vec3(0.0, mapSize, 0.0),
        glm::vec4(0.00f, 0.345f, 1.00f, 1.00f)
    );
    viewer.addRenderable(seaRenderable);
}

void initialize_test_scene( Viewer& viewer, MapGenerator& mapGenerator, float mapSize)    
{
    /*
     * Positionning the camera.
     */
    viewer.getCamera().setViewMatrix(glm::lookAt( 
                                        glm::vec3(mapSize/2.0, mapSize/2.0, 1.50*mapSize/2.0), 
                                        glm::vec3(mapSize/2.0, mapSize/2.0, 0), 
                                        glm::vec3( 0, 1, 0) 
                                    ) 
    );

    /*
     * Loading default shaders.
     */
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
            std::list<std::string>{
                "../shaders/flatVertex.vert", 
                "../shaders/flatFragment.frag"
            }
    );
    viewer.addShaderProgram(flatShader);

    /*
     * Creating the map renderable and adding it to the system.
     */
    Map2DRenderablePtr mapRenderable = std::make_shared<Map2DRenderable>(flatShader, mapGenerator);
    viewer.addRenderable(mapRenderable);

    //Default shader

    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    MaterialPtr pearl = Material::Pearl();

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    viewer.setDirectionalLight(directionalLight);

    //Textured shader
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{
        "../shaders/textureVertex.vert",
        "../shaders/textureFragment.frag"});
    viewer.addShaderProgram( texShader );

    BoidsManagerPtr boidsManager = std::make_shared<BoidsManager>(mapGenerator);

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemBoidPtr system = std::make_shared<DynamicSystemBoid>();
    SolverBoidPtr solver = std::make_shared<SolverBoid>();
    system->setSolver(solver);
    system->setDt(0.01);
    system->setBoidsManager(boidsManager);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemBoidRenderablePtr systemRenderable = std::make_shared<DynamicSystemBoidRenderable>(system);

    MovableBoidPtr leaderRabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(250, 300), random(250, 300), 2));
    leaderRabbit->setNewLeader(leaderRabbit);

    for (int i = 0; i < 6; ++i) {
        MovableBoidPtr rabbitFellow = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(250, 300), random(250, 300), 2));
        rabbitFellow->setNewLeader(leaderRabbit);
    }

    MovableBoidPtr leaderWolf = boidsManager->addMovableBoid(WOLF, glm::vec3(random(250, 300), random(250, 300), 2));
    leaderWolf->setNewLeader(leaderWolf);
    for (int i = 0; i < 4; ++i) {
        MovableBoidPtr wolfFellow = boidsManager->addMovableBoid(WOLF, glm::vec3(random(250, 300), random(250, 300), 2));
        wolfFellow->setNewLeader(leaderWolf);
    }

    for (int i = 0; i < 10; ++i) {
        boidsManager->addRootedBoid(CARROT, glm::vec3(random(250, 300), random(250, 300), 2));
    }

    for (int i = 0; i < 10; ++i) {
        boidsManager->addRootedBoid(TREE, glm::vec3(random(250, 300), random(250, 300), 2));
    }

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();

}
