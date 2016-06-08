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
#include "../include/boids2D/MovableBoidsRenderable.hpp"
#include "../include/boids2D/RootedBoidsRenderable.hpp"

#include "../include/graphicPrimitives/QuadRenderable.hpp"
#include "../include/terrain/MapGenerator.hpp"
#include "../include/terrain/MapRenderable.hpp"
#include "../include/terrain/SeaRenderable.hpp"

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


///< TODO : ajouté la vérif sur le boidsManager avec les boids déjà placé
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

void place_rabbit_group(Viewer& viewer, MapGenerator& mapGenerator, BoidsManagerPtr boidsManager, Biome biomeType)
{
    glm::vec3 positionMaster = compute_biome_positionLeader(mapGenerator, biomeType, 0, MAP_SIZE, 2.0);
    glm::vec3 positionFellow;

    MovableBoidPtr leaderRabbit = boidsManager->addMovableBoid(RABBIT, positionMaster, positionMaster);
    leaderRabbit->setNewLeader(leaderRabbit);

    int nbRabbit = randInt(NB_RABBIT_MIN, NB_RABBIT_MAX);
    glm::vec3 diffPos;

    for (int i = 0; i < nbRabbit; ++i) {
        positionFellow = compute_biome_positionFellow(mapGenerator, biomeType, positionMaster, 0, MAP_SIZE, 2.0, 10.0f);
        MovableBoidPtr rabbitFellow = boidsManager->addMovableBoid(RABBIT, positionFellow, positionMaster);
        rabbitFellow->setNewLeader(leaderRabbit);
    }
}

void place_wolf_group(Viewer& viewer, MapGenerator& mapGenerator, BoidsManagerPtr boidsManager, Biome biomeType)
{
    glm::vec3 positionMaster = compute_biome_positionLeader(mapGenerator, biomeType, 0, MAP_SIZE, 2.0);
    glm::vec3 positionFellow;

    MovableBoidPtr leaderWolf = boidsManager->addMovableBoid(WOLF, positionMaster, positionMaster);
    leaderWolf->setNewLeader(leaderWolf);

    int nbWolf = randInt(NB_WOLF_MIN, NB_WOLF_MAX);

    for (int i = 0; i < nbWolf; ++i) {
        positionFellow = compute_biome_positionFellow(mapGenerator, biomeType, positionMaster, 0, MAP_SIZE, 2.0, 10.0f);
        MovableBoidPtr wolfFellow = boidsManager->addMovableBoid(WOLF, positionFellow, positionMaster);
        wolfFellow->setNewLeader(leaderWolf);
    }
}

void place_forest(Viewer& viewer, MapGenerator& mapGenerator, BoidsManagerPtr boidsManager, Biome biomeType)
{
    glm::vec3 position = compute_biome_positionLeader(mapGenerator, biomeType, 0, MAP_SIZE, 2.0);
    boidsManager->addRootedBoid(TREE, position);

    glm::vec3 positionFellow;

    int nbTree = randInt(NB_TREE_MIN, NB_TREE_MAX);

    for (int i = 0; i < nbTree - 1; ++i) {
        positionFellow = compute_biome_positionFellow(mapGenerator, biomeType, position, 0, MAP_SIZE, 2.0, 25.0f);
        boidsManager->addRootedBoid(TREE, positionFellow);
    }
}

void place_carrot_field(Viewer& viewer, MapGenerator& mapGenerator, BoidsManagerPtr boidsManager, Biome biomeType)
{
    glm::vec3 position = compute_biome_positionLeader(mapGenerator, biomeType, 0, MAP_SIZE, 2.0);
    boidsManager->addRootedBoid(CARROT, position);

    glm::vec3 positionFellow;

    int nbCarrot = randInt(NB_CARROT_MIN, NB_CARROT_MAX);

    for (int i = 0; i < nbCarrot - 1; ++i) {
        positionFellow = compute_biome_positionFellow(mapGenerator, biomeType, position, 0, MAP_SIZE, 2.0, 30.0f);
        boidsManager->addRootedBoid(CARROT, positionFellow);
    }
}

void place_boid( Viewer& viewer, MapGenerator& mapGenerator, BoidsManagerPtr boidsManager, Biome biomeType,
    int nbRabbitGroup, int nbWolfGroup, int nbForest,  int nbCarrotField) 
{
    for(int i = 0; i < nbRabbitGroup; i++) {
        place_rabbit_group(viewer, mapGenerator, boidsManager, biomeType);
    }

    for(int i = 0; i < nbWolfGroup; i++) {
        place_wolf_group(viewer, mapGenerator, boidsManager, biomeType);
    }

    for(int i = 0; i < nbForest; i++) {
        place_forest(viewer, mapGenerator, boidsManager, biomeType);
    }

    for(int i = 0; i < nbCarrotField; i++) {
        place_carrot_field(viewer, mapGenerator, boidsManager, biomeType);
    }
}

void display_2Dboids( Viewer& viewer, BoidsManagerPtr boidsManager, 
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

    for(RootedBoidPtr m : boidsManager->getAllRootedBoids())
    {
        BoidRenderablePtr br = std::make_shared<BoidRenderable>(texShader, m);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }
}

void display_3Dboids( Viewer& viewer, BoidsManagerPtr boidsManager, 
    DynamicSystemBoidRenderablePtr systemRenderable, ShaderProgramPtr instanceShader, ShaderProgramPtr flatShader)
{
    MovableBoidsRenderablePtr rabbitsRenderable = std::make_shared<MovableBoidsRenderable>(instanceShader, 
        boidsManager, RABBIT, "../meshes/rabbit.obj", "../textures/rabbit_texture.png");
    rabbitsRenderable->setMaterial(Material::Pearl());
    viewer.addRenderable(rabbitsRenderable);

    MovableBoidsRenderablePtr wolfsRenderable = std::make_shared<MovableBoidsRenderable>(instanceShader, 
        boidsManager, WOLF, "../meshes/wolf.obj", "../textures/wolf_texture.png");
    wolfsRenderable->setMaterial(Material::Pearl());
    viewer.addRenderable(wolfsRenderable);

    RootedBoidsRenderablePtr treesRenderable = std::make_shared<RootedBoidsRenderable>(instanceShader, 
        boidsManager, TREE, "../meshes/trunk.obj", "../textures/trunk_texture.png");
    treesRenderable->setMaterial(Material::Bronze());

    RootedBoidsRenderablePtr leafsRenderable = std::make_shared<RootedBoidsRenderable>(instanceShader, 
        boidsManager, TREE, "../meshes/leaf.obj", "../textures/leaf_texture.png");
    treesRenderable->setMaterial(Material::Pearl());
    viewer.addRenderable(leafsRenderable); 
    viewer.addRenderable(treesRenderable);    

    RootedBoidsRenderablePtr carrotsRenderable = std::make_shared<RootedBoidsRenderable>(instanceShader, 
        boidsManager, CARROT, "../meshes/carrot.obj", "../textures/carrot_texture.png");
    treesRenderable->setMaterial(Material::Pearl());
    viewer.addRenderable(carrotsRenderable); 

    #ifdef DEBUG
    for(MovableBoidPtr m : boidsManager->getMovableBoids())
    {
        SightRenderablePtr sight = std::make_shared<SightRenderable>(flatShader, m);
        viewer.addRenderable(sight);

        StateRenderablePtr state = std::make_shared<StateRenderable>(flatShader, m);
        viewer.addRenderable(state);
    }
    #endif
}

void initialize_map2D(Viewer& viewer, MapGenerator& mapGenerator, float mapSize)
{    /*
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
   ShaderProgramPtr seaShader = std::make_shared<ShaderProgram>(
            std::list<std::string>{
                "../shaders/sea.vert", 
                "../shaders/sea.frag"
            }
    );
    viewer.addShaderProgram(seaShader);

    ShaderProgramPtr mapShader = std::make_shared<ShaderProgram>(
            std::list<std::string>{
                "../shaders/map.vert", 
                "../shaders/map.tcs", 
                "../shaders/map.tes", 
                "../shaders/map.geom", 
                "../shaders/map.frag"
            }
    );
    viewer.addShaderProgram(mapShader);


    /*
     * Creating the material
     */
    glm::vec3 mAmbient(0.8), mDiffuse(0.3), mSpecular(0.0);
    float mShininess=0.125*128;
    MaterialPtr material = std::make_shared<Material>(mAmbient, mDiffuse, mSpecular, mShininess);
    /*
     * Creating the map renderable and adding it to the system.
     */
    MapRenderablePtr mapRenderable = std::make_shared<MapRenderable>(mapShader, mapGenerator);
    mapRenderable->setMaterial(material);
    viewer.addRenderable(mapRenderable);
    
    /*
     * Creating a QuadRenderable at the altitude 0, so as to represent a calm sea.
     */
    SeaRenderablePtr seaRenderable = std::make_shared<SeaRenderable>(
        seaShader,
	mapGenerator,
        glm::vec3(0.0, 0.0, 0.1),
        glm::vec3(mapSize, 0.0, 0.1),
        glm::vec3(mapSize, mapSize, 0.1),
        glm::vec3(0.0, mapSize, 0.1)
    );
    seaRenderable->setMaterial(material);
    viewer.addRenderable(seaRenderable);



    //Define a point light
    glm::vec3 p_position(0.0,0.0,0.0), p_ambient(0.0,0.0,0.0), p_diffuse(0.0,0.0,0.0), p_specular(0.0,0.0,0.0);
    float p_constant=0.0, p_linear=0.0, p_quadratic=0.0;

    p_position = glm::vec3(mapSize/2, mapSize/2, mapSize/2);
    p_ambient = glm::vec3(1.0, 1.0, 1.0);
    p_diffuse = glm::vec3(1.0,1.0,1.0);
    p_specular = glm::vec3(1.0,1.0,1.0);
    p_constant=1.0;
    p_linear=8e-4;
    p_quadratic=0.0;
    PointLightPtr pointLight1 = std::make_shared<PointLight>(p_position, p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
    viewer.addPointLight(pointLight1);
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
     * Loading default shaders.
     */
   ShaderProgramPtr seaShader = std::make_shared<ShaderProgram>(
            std::list<std::string>{
                "../shaders/sea.vert", 
                "../shaders/sea.frag"
            }
    );
    viewer.addShaderProgram(seaShader);

    ShaderProgramPtr mapShader = std::make_shared<ShaderProgram>(
            std::list<std::string>{
                "../shaders/map.vert", 
                "../shaders/map.tcs", 
                "../shaders/map.tes", 
                "../shaders/map.geom", 
                "../shaders/map.frag"
            }
    );
    viewer.addShaderProgram(mapShader);


    /*
     * Creating the material
     */
    glm::vec3 mAmbient(0.8), mDiffuse(0.3), mSpecular(0.0);
    float mShininess=0.125*128;
    MaterialPtr material = std::make_shared<Material>(mAmbient, mDiffuse, mSpecular, mShininess);
    /*
     * Creating the map renderable and adding it to the system.
     */
    MapRenderablePtr mapRenderable = std::make_shared<MapRenderable>(mapShader, mapGenerator);
    mapRenderable->setMaterial(material);
    viewer.addRenderable(mapRenderable);
    
    /*
     * Creating a QuadRenderable at the altitude 0, so as to represent a calm sea.
     */
    SeaRenderablePtr seaRenderable = std::make_shared<SeaRenderable>(
        seaShader,
	mapGenerator,
        glm::vec3(0.0, 0.0, 0.1),
        glm::vec3(mapSize, 0.0, 0.1),
        glm::vec3(mapSize, mapSize, 0.1),
        glm::vec3(0.0, mapSize, 0.1)
    );
    seaRenderable->setMaterial(material);
    viewer.addRenderable(seaRenderable);

    /*
     * Creating the map renderable and adding it to the system.
     */
    //MapRenderablePtr mapRenderable = std::make_shared<MapRenderable>(flatShader, mapGenerator);
    //viewer.addRenderable(mapRenderable);

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

    ShaderProgramPtr instanceShader = std::make_shared<ShaderProgram>(std::list<std::string>{
                "../shaders/instanceVertex.vert", 
                "../shaders/instanceFragment.frag"});
    viewer.addShaderProgram( instanceShader );

    BoidsManagerPtr boidsManager = std::make_shared<BoidsManager>(mapGenerator, viewer, flatShader);

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

    place_boid( viewer, mapGenerator, boidsManager, Plains, 10, 3, 10, 10); 

    /*
    MovableBoidPtr leaderRabbit = boidsManager->addMovableBoid(RABBIT, glm::vec3(random(300, 350), random(300, 350), 2));
    leaderRabbit->setNewLeader(leaderRabbit);
    
    for (int i = 0; i < 1; ++i) {
        MovableBoidPtr rabbitFellow = boidsManager->addMovableBoid(RABBIT, glm::vec3(0.5, 0.5, 0.0));
        rabbitFellow->setNewLeader(leaderRabbit);
    }
    
    
    MovableBoidPtr leaderWolf = boidsManager->addMovableBoid(WOLF, glm::vec3(random(300, 350), random(300, 350), 2));
    leaderWolf->setNewLeader(leaderWolf);
    for (int i = 0; i < 1; ++i) {
        MovableBoidPtr wolfFellow = boidsManager->addMovableBoid(WOLF, glm::vec3(0.0, 0.0, 0.5));
        wolfFellow->setNewLeader(leaderWolf);
    }
    

    for (int i = 0; i < 10; ++i) {
        boidsManager->addRootedBoid(CARROT, glm::vec3(random(300, 350), random(300, 350), 2));
    }

    for (int i = 0; i < 10; ++i) {
        boidsManager->addRootedBoid(TREE, glm::vec3(random(300, 350), random(300, 350), 2));
    }
    */

    //display_2Dboids(viewer, boidsManager, systemRenderable, texShader, flatShader);
    display_3Dboids(viewer, boidsManager, systemRenderable, instanceShader, flatShader);

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();

}
