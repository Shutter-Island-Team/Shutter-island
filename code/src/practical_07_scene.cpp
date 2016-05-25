#include "./../include/practical_07_scene.hpp"

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
#include "../include/boids2D/AuraRenderable.hpp"

#include <cstdlib>
#include <ctime>

#include "../include/boids2D/DynamicSystemBoid.hpp"
#include "../include/boids2D/DynamicSystemBoidRenderable.hpp"

#include "../include/Utils.hpp"

void initialize_practical_07_scene( Viewer& viewer )
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, -8, 8 ), glm::vec3(0, 0, 0), glm::vec3( 0, 0, 1 ) ) );

    //Default shader

    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{
	    "../shaders/flatVertex.vert", 
		"../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    //Add a 3D frame to the viewer
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    //Temporary variables
    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    std::string filename;
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

    ShaderProgramPtr multiTexShader = std::make_shared<ShaderProgram>(std::list<std::string>{
	    "../shaders/multiTextureVertex.vert",
		"../shaders/multiTextureFragment.frag"});
    viewer.addShaderProgram( multiTexShader );

    //Textured plane
    filename = "./../textures/grass_texture.png";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(10.0,10.0,10.0));
    texPlane->setParentTransform(parentTransformation);
    texPlane->setMaterial(pearl);
    viewer.addRenderable(texPlane);

    //Textured cube
    filename = "./../textures/mipmap1.png";
    TexturedCubeRenderablePtr texCube = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(-2,0.0,0.5));
    texCube->setParentTransform(parentTransformation);
    texCube->setMaterial(pearl);
    viewer.addRenderable(texCube);

    //Mipmap cube
    std::vector< std::string > filenames;
    filenames.push_back("./../textures/mipmap1.png");
    filenames.push_back("./../textures/mipmap2.png");
    filenames.push_back("./../textures/mipmap3.png");
    filenames.push_back("./../textures/mipmap4.png");
    filenames.push_back("./../textures/mipmap5.png");

    MipMapCubeRenderablePtr mipmapCube = std::make_shared<MipMapCubeRenderable>(texShader, filenames);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,0.5));
    mipmapCube->setParentTransform(parentTransformation);
    mipmapCube->setMaterial(pearl);
    viewer.addRenderable(mipmapCube);

    //Multi-textured cube
    std::string filename1="./../textures/crate.jpg", filename2="./../textures/halflife.png";
    MultiTexturedCubeRenderablePtr multitexCube = std::make_shared<MultiTexturedCubeRenderable>(multiTexShader, filename1, filename2);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    multitexCube->setParentTransform(parentTransformation);
    multitexCube->setMaterial(pearl);
    viewer.addRenderable(multitexCube);

    // textured bunny
    TexturedLightedMeshRenderablePtr bunny = std::make_shared<TexturedLightedMeshRenderable>( texShader, "../meshes/Bunny.obj", "../textures/TexturedBunny.png");
    bunny->setMaterial(pearl);
    parentTransformation = glm::translate( glm::mat4(1.0), glm::vec3( 0, 4, 1.0 ) );
    parentTransformation = glm::rotate( parentTransformation, float(M_PI_2), glm::vec3(1,0,0) );
    parentTransformation = glm::scale( parentTransformation, glm::vec3(2,2,2));
    bunny->setParentTransform( parentTransformation );
    viewer.addRenderable(bunny);

    viewer.startAnimation();
    viewer.setAnimationLoop(true, 4.0);
}

void initialize_boid_scene( Viewer& viewer )
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

    MovableBoidPtr mvb;
    BoidRenderablePtr br;
    MovableParameters* parameters;

    for (int i = 0; i < 50; ++i) {
        parameters = new MovableParameters();
        mvb = std::make_shared<MovableBoid>(glm::vec3(random(-15, 15), random(-15, 15), 2), RABBIT, parameters);
        mvb->associateBoid(mvb);
        boidsManager->addMovableBoid(mvb);
        br = std::make_shared<BoidRenderable>(texShader, mvb);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }

    for (int i = 0; i < 10; ++i) {
        parameters = new MovableParameters();
        mvb = std::make_shared<MovableBoid>(glm::vec3(random(-15, 15), random(-15, 15), 2), WOLF, parameters);
        mvb->associateBoid(mvb);
        boidsManager->addMovableBoid(mvb);
        br = std::make_shared<BoidRenderable>(texShader, mvb);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}

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

    RabbitPtr rabbitBoid;
    WolfPtr wolfBoid;
    CarrotPtr carrotBoid;
    TreePtr treeBoid;
    BoidRenderablePtr br;
    MovableParameters* parameters;

    for (int i = 0; i < 10; ++i) {
        parameters = new MovableParameters();
        rabbitBoid = std::make_shared<Rabbit>(glm::vec3(random(-15, 15), random(-15, 15), 2), parameters);
        rabbitBoid->associateBoid(rabbitBoid);
        boidsManager->addMovableBoid(rabbitBoid);
        br = std::make_shared<BoidRenderable>(texShader, rabbitBoid);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }

    for (int i = 0; i < 10; ++i) {
        parameters = new MovableParameters();
        wolfBoid = std::make_shared<Wolf>(glm::vec3(random(-15, 15), random(-15, 15), 2), parameters);
        wolfBoid->associateBoid(wolfBoid);
        boidsManager->addMovableBoid(wolfBoid);
        br = std::make_shared<BoidRenderable>(texShader, wolfBoid);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }

    for (int i = 0; i < 10; ++i) {
        carrotBoid = std::make_shared<Carrot>(glm::vec3(random(-15, 15), random(-15, 15), 2));
        boidsManager->addRootedBoid(carrotBoid);
        br = std::make_shared<BoidRenderable>(texShader, carrotBoid);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }

    for (int i = 0; i < 10; ++i) {
        treeBoid = std::make_shared<Tree>(glm::vec3(random(-15, 15), random(-15, 15), 2));
        boidsManager->addRootedBoid(treeBoid);
        br = std::make_shared<BoidRenderable>(texShader, treeBoid);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }

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

    MovableParameters* parameters;

    parameters = new MovableParameters();
    MovableBoidPtr mvb1 = std::make_shared<MovableBoid>(glm::vec3(-5, -5, 2), glm::vec3(1, 0, 0), RABBIT, parameters);
    mvb1->associateBoid(mvb1);
    boidsManager->addMovableBoid(mvb1);

    BoidRenderablePtr br1 = std::make_shared<BoidRenderable>(texShader, mvb1);
    br1->setMaterial(pearl);
    SightRenderablePtr sight1 = std::make_shared<SightRenderable>(flatShader, mvb1);
    HierarchicalRenderable::addChild( br1, sight1 );

    parameters = new MovableParameters();
    MovableBoidPtr mvb2 = std::make_shared<MovableBoid>(glm::vec3(0, -1, 2), glm::vec3(0, -1, 0), RABBIT, parameters);
    mvb2->associateBoid(mvb2);
    boidsManager->addMovableBoid(mvb2);
    
    BoidRenderablePtr br2 = std::make_shared<BoidRenderable>(texShader, mvb2);
    br2->setMaterial(pearl);
    SightRenderablePtr sight2 = std::make_shared<SightRenderable>(flatShader, mvb2);
    HierarchicalRenderable::addChild( br2, sight2 );

    HierarchicalRenderable::addChild( systemRenderable, br1 );
    HierarchicalRenderable::addChild( systemRenderable, br2 );

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

    MovableParameters* parameters;

    parameters = new MovableParameters();
    MovableBoidPtr mvb1 = std::make_shared<MovableBoid>(glm::vec3(-5, -5, 2), glm::vec3(1, 0, 0), RABBIT, parameters);
    mvb1->associateBoid(mvb1);
    boidsManager->addMovableBoid(mvb1);

    BoidRenderablePtr br1 = std::make_shared<BoidRenderable>(texShader, mvb1);
    br1->setMaterial(pearl);
    SightRenderablePtr sight1 = std::make_shared<SightRenderable>(flatShader, mvb1);
    HierarchicalRenderable::addChild( br1, sight1 );

    parameters = new MovableParameters();
    MovableBoidPtr mvb2 = std::make_shared<MovableBoid>(glm::vec3(0, -1, 2), glm::vec3(0, -1, 0), RABBIT, parameters);
    mvb2->associateBoid(mvb2);
    boidsManager->addMovableBoid(mvb2);
    
    BoidRenderablePtr br2 = std::make_shared<BoidRenderable>(texShader, mvb2);
    br2->setMaterial(pearl);
    SightRenderablePtr sight2 = std::make_shared<SightRenderable>(flatShader, mvb2);
    HierarchicalRenderable::addChild( br2, sight2 );

    HierarchicalRenderable::addChild( systemRenderable, br1 );
    HierarchicalRenderable::addChild( systemRenderable, br2 );

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

    RabbitPtr rabbitBoid;
    MovableParameters* parameters;
    BoidRenderablePtr br;
    SightRenderablePtr sight;

    for (int i = 0; i < 100; ++i) {
        parameters = new MovableParameters();
        rabbitBoid = std::make_shared<Rabbit>(glm::vec3(random(-15, 15), random(-15, 15), 2), parameters);
        rabbitBoid->associateBoid(rabbitBoid);
        boidsManager->addMovableBoid(rabbitBoid);
        br = std::make_shared<BoidRenderable>(texShader, rabbitBoid);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
        sight = std::make_shared<SightRenderable>(flatShader, rabbitBoid);
        HierarchicalRenderable::addChild( br, sight );
    }

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}

void test_create_MovableParameters_from_file() {
    MovableParameters* parameters = new MovableParameters(WOLF);

    std::cout << "MaxSpeed lu (3.5 attendu) : " << parameters->getMaxSpeed() << std::endl;
}

void initialize_boid_scene_walkstate( Viewer& viewer)    
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

    
    BoidRenderablePtr br;
    MovableParameters* parameters;
/*
    RabbitPtr rabbitBoid;
    for (int i = 0; i < 10; ++i) {
        parameters = new MovableParameters();
        rabbitBoid = std::make_shared<Rabbit>(glm::vec3(random(-15, 15), random(-15, 15), 2), parameters);
        rabbitBoid->associateBoid(rabbitBoid);
        boidsManager->addMovableBoid(rabbitBoid);
        br = std::make_shared<BoidRenderable>(texShader, rabbitBoid);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }
*/
    std::vector< WolfPtr > wolfBoids;
    for(int i = 0; i < 1; ++i)
    {
        parameters = new MovableParameters();
        wolfBoids.push_back(std::make_shared<Wolf>(glm::vec3(random(-15, 15), random(-15, 15), 2), parameters));
        wolfBoids[i]->associateBoid(wolfBoids[i]);
        wolfBoids[i]->getParameters().setNewLeader(wolfBoids[0]);
        boidsManager->addMovableBoid(wolfBoids[i]);
        br = std::make_shared<BoidRenderable>(texShader, wolfBoids[i]);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }
/*
    CarrotPtr carrotBoid;
    for (int i = 0; i < 10; ++i) {
        carrotBoid = std::make_shared<Carrot>(glm::vec3(random(-15, 15), random(-15, 15), 2));
        boidsManager->addRootedBoid(carrotBoid);
        br = std::make_shared<BoidRenderable>(texShader, carrotBoid);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }

    TreePtr treeBoid;
    for (int i = 0; i < 10; ++i) {
        treeBoid = std::make_shared<Tree>(glm::vec3(random(-15, 15), random(-15, 15), 2));
        boidsManager->addRootedBoid(treeBoid);
        br = std::make_shared<BoidRenderable>(texShader, treeBoid);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }
*/
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

    RabbitPtr rabbitBoid;
    BoidRenderablePtr br;
    MovableParameters* parameters;

    for (int i = 0; i < 2; ++i) {
        parameters = new MovableParameters();
        rabbitBoid = std::make_shared<Rabbit>(glm::vec3(random(-15, 15), random(-15, 15), 2), parameters);
        rabbitBoid->associateBoid(rabbitBoid);
        boidsManager->addMovableBoid(rabbitBoid);
        br = std::make_shared<BoidRenderable>(texShader, rabbitBoid);
        br->setMaterial(pearl);
        HierarchicalRenderable::addChild( systemRenderable, br );

        AuraRenderablePtr aura = std::make_shared<AuraRenderable>(texShader, rabbitBoid);
        HierarchicalRenderable::addChild ( br, aura );
    }

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

    MovableParameters* parameters;

    parameters = new MovableParameters();
    MovableBoidPtr mvb = std::make_shared<MovableBoid>(glm::vec3(-5, -5, 2), glm::vec3(1, 0, 0), RABBIT, parameters);
    mvb->associateBoid(mvb);
    boidsManager->addMovableBoid(mvb);

    BoidRenderablePtr br = std::make_shared<BoidRenderable>(texShader, mvb);
    br->setMaterial(pearl);

    SightRenderablePtr sight = std::make_shared<SightRenderable>(flatShader, mvb);
    HierarchicalRenderable::addChild( br, sight );
    HierarchicalRenderable::addChild( systemRenderable, br );

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}
