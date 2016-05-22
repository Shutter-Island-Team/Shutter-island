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
#include "../include/boids2D/BoidRenderable.hpp"
#include "../include/boids2D/BoidsManager.hpp"

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
    glm::vec3 lightPosition(0.0,0.0,5.0);
    DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    directionalLightRenderable->setLocalTransform(localTransformation);
    viewer.setDirectionalLight(directionalLight);
    viewer.addRenderable(directionalLightRenderable);

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

    //Textured plane
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

    for (int i = 0; i < 50; ++i) {
        mvb = std::make_shared<MovableBoid>(glm::vec3(random(-15, 15), random(-15, 15), 2), RABBIT);
        boidsManager->addMovableBoid(mvb);
        br = std::make_shared<BoidRenderable>(flatShader, mvb);
        HierarchicalRenderable::addChild( systemRenderable, br );
    }

    for (int i = 0; i < 10; ++i) {
        mvb = std::make_shared<MovableBoid>(glm::vec3(random(-15, 15), random(-15, 15), 2), WOLF);
        boidsManager->addMovableBoid(mvb);
        br = std::make_shared<BoidRenderable>(flatShader, mvb);
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

    //Textured plane
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

    RabbitPtr rbb;
    BoidRenderablePtr br;

    for (int i = 0; i < 50; ++i) {
        // rbb = std::make_shared<RabbitPtr>(glm::vec3(random(-15, 15), random(-15, 15), 2));
        // boidsManager->addMovableBoid(rbb);
        // br = std::make_shared<BoidRenderable>(flatShader, rbb);
        // HierarchicalRenderable::addChild( systemRenderable, br );
    }

    // WolfPtr wbb;

    // for (int i = 0; i < 10; ++i) {
    //     wbb = std::make_shared<WolfPtr>(glm::vec3(random(-15, 15), random(-15, 15), 2));
    //     boidsManager->addMovableBoid(wbb);
    //     br = std::make_shared<BoidRenderable>(flatShader, wbb);
    //     HierarchicalRenderable::addChild( systemRenderable, br );
    // }

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

    //Textured plane
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

    MovableBoidPtr mvb1 = std::make_shared<MovableBoid>(glm::vec3(-5, -5, 2), glm::vec3(-1, 0, 0), RABBIT);
    boidsManager->addMovableBoid(mvb1);
    MovableBoidPtr mvb2 = std::make_shared<MovableBoid>(glm::vec3(5, -5, 2), glm::vec3(1, 0, 0), RABBIT);
    boidsManager->addMovableBoid(mvb2);

    BoidRenderablePtr br1 = std::make_shared<BoidRenderable>(flatShader, mvb1);
    BoidRenderablePtr br2 = std::make_shared<BoidRenderable>(flatShader, mvb2);

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

    //Textured plane
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

    MovableBoidPtr mvb1 = std::make_shared<MovableBoid>(glm::vec3(-5, -5, 2), glm::vec3(1, 0, 0), RABBIT);
    boidsManager->addMovableBoid(mvb1);
    MovableBoidPtr mvb2 = std::make_shared<MovableBoid>(glm::vec3(0, -1, 2), glm::vec3(0, -1, 0), RABBIT);
    boidsManager->addMovableBoid(mvb2);

    BoidRenderablePtr br1 = std::make_shared<BoidRenderable>(flatShader, mvb1);
    BoidRenderablePtr br2 = std::make_shared<BoidRenderable>(flatShader, mvb2);

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

    //Textured plane
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

    MovableBoidPtr mvb = std::make_shared<MovableBoid>(glm::vec3(-5, -5, 2), glm::vec3(1, 0, 0), RABBIT);
    boidsManager->addMovableBoid(mvb);

    BoidRenderablePtr br = std::make_shared<BoidRenderable>(flatShader, mvb);

    HierarchicalRenderable::addChild( systemRenderable, br );

    viewer.addRenderable(systemRenderable);
    viewer.startAnimation();
}