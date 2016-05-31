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

#define MAP_SIZE 500.0
#define NB_RABBIT_MIN 6
#define NB_RABBIT_MAX 10
#define NB_WOLF_MIN 2
#define NB_WOLF_MAX 15
#define NB_TREE_MIN 8
#define NB_TREE_MAX 15
#define NB_CARROT_MIN 8
#define NB_CARROT_MAX 15

#include <cstdlib>
#include <ctime>

#include "../include/boids2D/DynamicSystemBoid.hpp"
#include "../include/boids2D/DynamicSystemBoidRenderable.hpp"

#include "../include/Utils.hpp"


// TODO : ajouté la vérif sur le boidsManager avec les boids déjà placé
glm::vec3 compute_biome_positionLeader(MapGenerator& mapGenerator, Biome biome, float min, float max, float zPos)
{
    glm::vec3 position(random(min, max), random(min, max), zPos);

    while(mapGenerator.getBiome(position.x, position.y) != biome) {
        position = glm::vec3(random(min, max), random(min, max), zPos);
    }

    return position;
}

glm::vec3 compute_biome_positionFellow(MapGenerator& mapGenerator, Biome biome, 
    glm::vec3 position, float min, float max, float zPos, float raduis)
{
    glm::vec3 positionFellow(random(min, max), random(min, max), zPos);

    while(glm::length(positionFellow - position) >= raduis || mapGenerator.getBiome(positionFellow.x, positionFellow.y) != biome) {
        positionFellow = glm::vec3(random(min, max), random(min, max), zPos);
    }

    return positionFellow;
}

void place_rabbit_group(Viewer& viewer, MapGenerator& mapGenerator, BoidsManagerPtr boidsManager )
{
    glm::vec3 position = compute_biome_positionLeader(mapGenerator, Plains, 0, MAP_SIZE, 2.0);
    glm::vec3 positionFellow;

    MovableBoidPtr leaderRabbit = boidsManager->addMovableBoid(RABBIT, position);
    leaderRabbit->setNewLeader(leaderRabbit);

    int nbRabbit = randInt(NB_RABBIT_MIN, NB_RABBIT_MAX);
    glm::vec3 diffPos;

    for (int i = 0; i < nbRabbit; ++i) {
        positionFellow = compute_biome_positionFellow(mapGenerator, Plains, position, 0, MAP_SIZE, 2.0, 10.0f);
        MovableBoidPtr rabbitFellow = boidsManager->addMovableBoid(RABBIT, positionFellow);
        rabbitFellow->setNewLeader(leaderRabbit);
    }
}

void place_wolf_group(Viewer& viewer, MapGenerator& mapGenerator, BoidsManagerPtr boidsManager )
{
    glm::vec3 position = compute_biome_positionLeader(mapGenerator, Plains, 0, MAP_SIZE, 2.0);
    glm::vec3 positionFellow;

    MovableBoidPtr leaderWolf = boidsManager->addMovableBoid(WOLF, position);
    leaderWolf->setNewLeader(leaderWolf);

    int nbWolf = randInt(NB_WOLF_MIN, NB_WOLF_MAX);

    for (int i = 0; i < nbWolf; ++i) {
        positionFellow = compute_biome_positionFellow(mapGenerator, Plains, position, 0, MAP_SIZE, 2.0, 10.0f);
        MovableBoidPtr wolfFellow = boidsManager->addMovableBoid(WOLF, positionFellow);
        wolfFellow->setNewLeader(leaderWolf);
    }
}

void place_forest(Viewer& viewer, MapGenerator& mapGenerator, BoidsManagerPtr boidsManager )
{
    glm::vec3 position = compute_biome_positionLeader(mapGenerator, Plains, 0, MAP_SIZE, 2.0);
    boidsManager->addRootedBoid(TREE, position);

    glm::vec3 positionFellow;

    int nbTree = randInt(NB_TREE_MIN, NB_TREE_MAX);

    for (int i = 0; i < nbTree - 1; ++i) {
        positionFellow = compute_biome_positionFellow(mapGenerator, Plains, position, 0, MAP_SIZE, 2.0, 25.0f);
        boidsManager->addRootedBoid(TREE, positionFellow);
    }
}

void place_carrot_field(Viewer& viewer, MapGenerator& mapGenerator, BoidsManagerPtr boidsManager )
{
    glm::vec3 position = compute_biome_positionLeader(mapGenerator, Plains, 0, MAP_SIZE, 2.0);
    boidsManager->addRootedBoid(CARROT, position);

    glm::vec3 positionFellow;

    int nbCarrot = randInt(NB_CARROT_MIN, NB_CARROT_MAX);

    for (int i = 0; i < nbCarrot - 1; ++i) {
        positionFellow = compute_biome_positionFellow(mapGenerator, Plains, position, 0, MAP_SIZE, 2.0, 30.0f);
        boidsManager->addRootedBoid(CARROT, positionFellow);
    }
}

void place_boid( Viewer& viewer, MapGenerator& mapGenerator, BoidsManagerPtr boidsManager, 
    int nbRabbitGroup, int nbWolfGroup, int nbForest,  int nbCarrotField) 
{
    for(int i = 0; i < nbRabbitGroup; i++) {
        place_rabbit_group(viewer, mapGenerator, boidsManager);
    }

    for(int i = 0; i < nbWolfGroup; i++) {
        place_wolf_group(viewer, mapGenerator, boidsManager);
    }

    for(int i = 0; i < nbForest; i++) {
        place_forest(viewer, mapGenerator, boidsManager);
    }

    for(int i = 0; i < nbCarrotField; i++) {
        place_carrot_field(viewer, mapGenerator, boidsManager);
    }
}

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

    place_boid( viewer, mapGenerator, boidsManager, 5, 5, 5, 5); 

    /*
    MovableBoidPtr leaderRabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(300, 350), random(300, 350), 2));
    leaderRabbit->setNewLeader(leaderRabbit);

    for (int i = 0; i < 6; ++i) {
        MovableBoidPtr rabbitFellow = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(300, 350), random(300, 350), 2));
        rabbitFellow->setNewLeader(leaderRabbit);
    }

    MovableBoidPtr leaderWolf = boidsManager->addMovableBoid(WOLF, glm::vec3(random(300, 350), random(300, 350), 2));
    leaderWolf->setNewLeader(leaderWolf);
    for (int i = 0; i < 4; ++i) {
        MovableBoidPtr wolfFellow = boidsManager->addMovableBoid(WOLF, glm::vec3(random(300, 350), random(300, 350), 2));
        wolfFellow->setNewLeader(leaderWolf);
    }

    for (int i = 0; i < 10; ++i) {
        boidsManager->addRootedBoid(CARROT, glm::vec3(random(300, 350), random(300, 350), 2));
    }

    for (int i = 0; i < 10; ++i) {
        boidsManager->addRootedBoid(TREE, glm::vec3(random(300, 350), random(300, 350), 2));
    }
    */

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();

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
void initialize_map2D(Viewer& viewer)
{
    /*
     * Positionning the camera.
     */
    viewer.getCamera().setViewMatrix(glm::lookAt( 
                                        glm::vec3(MAP_SIZE/2.0, MAP_SIZE/2.0, 1.50*MAP_SIZE/2.0), 
                                        glm::vec3(MAP_SIZE/2.0, MAP_SIZE/2.0, 0), 
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
     * Creating the map generator and generating the map.
     */
    MapGenerator mapGenerator(MAP_SIZE);
    mapGenerator.compute();

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
        glm::vec3(MAP_SIZE, 0.0, 0.0),
        glm::vec3(MAP_SIZE, MAP_SIZE, 0.0),
        glm::vec3(0.0, MAP_SIZE, 0.0),
        glm::vec4(0.00f, 0.345f, 1.00f, 1.00f)
    );
    viewer.addRenderable(seaRenderable);
}

void initialize_test_scene( Viewer& viewer)    
{
    /*
     * Positionning the camera.
     */
    viewer.getCamera().setViewMatrix(glm::lookAt( 
                                        glm::vec3(MAP_SIZE/2.0, MAP_SIZE/2.0, 1.50*MAP_SIZE/2.0), 
                                        glm::vec3(MAP_SIZE/2.0, MAP_SIZE/2.0, 0), 
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
     * Creating the map generator and generating the map.
     */
    MapGenerator mapGenerator(MAP_SIZE);
    mapGenerator.compute();

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

    float boundaryInfX = 200;
    float boundarySupX = 350;
    float boundaryInfY = 200;
    float boundarySupY = 350;

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemBoidRenderablePtr systemRenderable = std::make_shared<DynamicSystemBoidRenderable>(system);

    MovableBoidPtr leaderRabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(boundaryInfX, boundarySupX), random(boundaryInfY, boundarySupY), 2));
    leaderRabbit->setNewLeader(leaderRabbit);

    for (int i = 0; i < 1; ++i) {
        MovableBoidPtr rabbitFellow = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(boundaryInfX, boundarySupX), random(boundaryInfY, boundarySupY), 2));
        rabbitFellow->setNewLeader(leaderRabbit);
    }

    MovableBoidPtr leaderWolf = boidsManager->addMovableBoid(WOLF, glm::vec3(random(boundaryInfX, boundarySupX), random(boundaryInfY, boundarySupY), 2));
    leaderWolf->setNewLeader(leaderWolf);
    for (int i = 0; i < 1; ++i) {
        MovableBoidPtr wolfFellow = boidsManager->addMovableBoid(WOLF, glm::vec3(random(boundaryInfX, boundarySupX), random(boundaryInfY, boundarySupY), 2));
        wolfFellow->setNewLeader(leaderWolf);
    }

    for (int i = 0; i < 10; ++i) {
        boidsManager->addRootedBoid(CARROT, glm::vec3(random(boundaryInfX, boundarySupX), random(boundaryInfY, boundarySupY), 2));
    }

    for (int i = 0; i < 2; ++i) {
        boidsManager->addRootedBoid(TREE, glm::vec3(random(boundaryInfX, boundarySupX), random(boundaryInfY, boundarySupY), 2));
    }

    display_boid(viewer, boidsManager, systemRenderable, texShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}
