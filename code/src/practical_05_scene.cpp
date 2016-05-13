#include "../include/practical_05_scene.hpp"

#include "../include/ShaderProgram.hpp"

#include "../include/QuadRenderable.hpp"
#include "../include/FrameRenderable.hpp"

#include "../include/dynamics/DampingForceField.hpp"
#include "../include/dynamics/ConstantForceField.hpp"
#include "../include/dynamics/SpringForceField.hpp"
#include "../include/dynamics/EulerExplicitSolver.hpp"

#include "../include/dynamics/RoundedParticleRenderable.hpp"
#include "../include/dynamics/ParticleListRenderable.hpp"
#include "../include/dynamics/ConstantForceFieldRenderable.hpp"
#include "../include/dynamics/SpringForceFieldRenderable.hpp"
#include "../include/dynamics/SpringListRenderable.hpp"
#include "../include/dynamics/ControlledForceFieldRenderable.hpp"


#include "../include/lighting/LightedMeshRenderable.hpp"
#include "../include/lighting/LightedCubeRenderable.hpp"
#include "../include/lighting/LightedCylinderRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/lighting/PointLightRenderable.hpp"
#include "../include/lighting/SpotLightRenderable.hpp"


#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"
#include "../include/texturing/MultiTexturedCubeRenderable.hpp"
#include "../include/texturing/MipMapCubeRenderable.hpp"
#include "../include/texturing/TexturedLightedMeshRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"

void initialize_practical_05_scene( Viewer& viewer )
{
    //Set up a shader and add a 3D frame.
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/flatVertex.vert", "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemPtr system = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    system->setSolver(solver);
    system->setDt(0.01);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
    viewer.addRenderable(systemRenderable);

    //Populate the dynamic system with particles, forcefields and create renderables associated to them for visualization.
    //Uncomment only one of the following line

    //practical05_particles(viewer, system, systemRenderable);
    //practical05_springs(viewer, system, systemRenderable);
    //practical05_collisions(viewer, system, systemRenderable);
    practical05_playPool(viewer, system, systemRenderable);

    //Finally activate animation
    viewer.startAnimation();
}

void practical05_particles(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable)
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3( 0, -6, 0 ), glm::vec3(0,0,0), glm::vec3(0,0,1)));

    //Initialize a shader for the following renderables
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/flatVertex.vert", "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    //We diminish the time step to be able to see what happens before particles go too far
    system->setDt(5e-4);

    //Deactivate collision detection
    system->setCollisionsDetection(false);

    glm::vec3 px,pv;
    float pm, pr;
    //RoundedParticles with gravity and damping
    {
        //Initialize a particle with position, velocity, mass and radius and add it to the system
        px = glm::vec3(0.0,0.0,1.0);
        pv = glm::vec3(3.0,0.0,0.0);
        pr = 0.1;
        pm = 1.0;
        RoundedParticlePtr particle1 = std::make_shared<RoundedParticle>( px, pv, pm, pr);
        system->addParticle( particle1 );

        px = glm::vec3(0.0,0.0,1.5);
        pv = glm::vec3(6.0,0.0,0.0);
        pr = 0.1;
        pm = 1.0;
        RoundedParticlePtr particle2 = std::make_shared<RoundedParticle>( px, pv, pm, pr);
        system->addParticle( particle2 );

        //Initialize a force field that apply to all the particles of the system to simulate gravity
        //Add it to the system as a force field
        ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(system->getParticles(), glm::vec3{0,0,-10} );
        system->addForceField( gravityForceField );

        //Create a particleRenderable for each particle of the system
        //DynamicSystemRenderable act as a hierarchical renderable
        //This which allows to easily apply transformation on the visualiazation of a dynamicSystem
        RoundedParticleRenderablePtr particleRenderable1 = std::make_shared<RoundedParticleRenderable>(flatShader, particle1);
        HierarchicalRenderable::addChild( systemRenderable, particleRenderable1 );
        RoundedParticleRenderablePtr particleRenderable2 = std::make_shared<RoundedParticleRenderable>(flatShader, particle2);
        HierarchicalRenderable::addChild( systemRenderable, particleRenderable2 );
    }
}


void practical05_springs(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable)
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3( -10, 0, 2 ), glm::vec3(0,0,0), glm::vec3(0,0,1)));

    //Initialize a shader for the following renderables
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/flatVertex.vert", "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    //Initialize particle attributes (radius, mass)
    float pr = 0.1, pm = 10.0;
    glm::vec3 px(0.0,0.0,0.0), pv(0.0,0.0,0.0);

    //Create particles on a squared uniform grid starting at origin
    std::vector<ParticlePtr> particles;
    glm::vec3 origin(0,-2.0,1.0), displacement(0.0,0.0,0.0);
    int particlePerLine = 11;
    float gridWidth=4.0, gridHeight=4.0;
    float ystep = gridWidth / (float)(particlePerLine-1);
    float zstep = gridHeight / (float)(particlePerLine-1);
    particles.resize(particlePerLine*particlePerLine);
    for( size_t i = 0; i < particlePerLine; ++ i )
    {
        for( size_t j = 0; j < particlePerLine; ++ j )
        {
            displacement = glm::vec3(0, i*ystep, j*zstep);
            px = origin + displacement;
            particles[i*particlePerLine+j] = std::make_shared<RoundedParticle>( px, pv, pm, pr );
            system->addParticle( particles[i*particlePerLine+j] );
        }
    }

    //Fix particles on the y-borders
    for( size_t j = 0; j < particlePerLine; ++ j )
    {
        particles[0*particlePerLine+j]->setFixed( true );
        particles[10*particlePerLine+j]->setFixed( true );
    }

    //Initialize springs attributes (stiffness, rest length, damping)
    float stiffness = 4e3, l0 = gridWidth / (particlePerLine-1), damping = 0.0;

    //Create springs between particles of the grid, horizontally and vertically
    //Store them in a list
    std::list<SpringForceFieldPtr> springForceFields;
    //Interior
    for( size_t i = 1; i < particlePerLine; ++ i )
    {
        for( size_t j = 1; j < particlePerLine; ++ j )
        {
            SpringForceFieldPtr spring1 = std::make_shared<SpringForceField>( particles[(i-1)*particlePerLine+j], particles[i*particlePerLine+j], stiffness, l0, damping );
            springForceFields.push_back(spring1);
            system->addForceField( spring1 );
            SpringForceFieldPtr spring2 = std::make_shared<SpringForceField>( particles[i*particlePerLine+(j-1)], particles[i*particlePerLine+j], stiffness, l0, damping );
            springForceFields.push_back(spring2);
            system->addForceField( spring2 );
        }
    }
    //Border case 1
    for( size_t j = 1; j < particlePerLine; ++ j )
    {
        SpringForceFieldPtr spring = std::make_shared<SpringForceField>( particles[0*particlePerLine+j], particles[0*particlePerLine+(j-1)], stiffness, l0, damping );
        springForceFields.push_back(spring);
        system->addForceField( spring );
    }
    //Border case 2
    for( size_t i = 1; i < particlePerLine; ++ i )
    {
        SpringForceFieldPtr spring = std::make_shared<SpringForceField>( particles[(i-1)*particlePerLine+0], particles[i*particlePerLine+0], stiffness, l0, damping );
        springForceFields.push_back(spring);
        system->addForceField( spring );
    }

    //Initialize a force field that apply to all the particles of the system to simulate gravity
    //Add it to the system as a force field
    ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(system->getParticles(), glm::vec3{0,0,-10} );
    system->addForceField( gravityForceField );

    //Initialize a force field that apply to all the particles of the system to simulate vicosity (air friction)
    float dampingCoefficient = 0.0;
    DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(system->getParticles(), dampingCoefficient);
    system->addForceField(dampingForceField);


    //Create a particleListRenderable to efficiently visualize the particles of the system
    ParticleListRenderablePtr particleListRenderable = std::make_shared<ParticleListRenderable>( flatShader, particles);
    HierarchicalRenderable::addChild(systemRenderable, particleListRenderable);

    //Create a springListRenderable to efficiently visualize the springs of the system
    SpringListRenderablePtr springsRenderable = std::make_shared<SpringListRenderable>(flatShader, springForceFields);
    HierarchicalRenderable::addChild( systemRenderable, springsRenderable );

    //Display gravity
    ConstantForceFieldRenderablePtr gravityRenderable = std::make_shared<ConstantForceFieldRenderable>(flatShader, gravityForceField);
    HierarchicalRenderable::addChild( systemRenderable, gravityRenderable );
}

void practical05_collisions(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable)
{
    //Position the camera
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3( 0, 2, 2 ), glm::vec3(0,0,0), glm::vec3(0,0,1)));


    //Initialize a shader for the following renderables
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/flatVertex.vert", "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    //Activate collision detection
    system->setCollisionsDetection(true);

    //Initialize the restitution coefficient for collision
    //1.0 = full elastic response
    //0.0 = full absorption
    system->setRestitution(1.0f);

    //Initialize a plane from 3 points and add it to the system as an obstacle
    glm::vec3 p1(-1.0,-1.0,0.0),p2(1.0,-1.0,0.0), p3(1.0,1.0,0.0), p4(-1.0,1.0,0.0);
    PlanePtr plane = std::make_shared<Plane>(p1, p2, p3);
    system->addPlaneObstacle(plane);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable = std::make_shared<QuadRenderable>(flatShader, p1,p2,p3,p4);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable );

    glm::vec3 px,pv;
    float pm, pr;
    //RoundedParticle vs Plane collision
    {

        //Initialize a particle with position, velocity, mass and radius and add it to the system
        px = glm::vec3(0.0,0.0,1.0);
        pv = glm::vec3(0.0,0.0,0.0);
        pr = 0.1;
        pm = 1.0;
        RoundedParticlePtr particle = std::make_shared<RoundedParticle>( px, pv, pm, pr);
        system->addParticle( particle );

        //Create a particleRenderable for each particle of the system
        //DynamicSystemRenderable act as a hierarchical renderable
        //This which allows to easily apply transformation on the visualiazation of a dynamicSystem
        RoundedParticleRenderablePtr particleRenderable = std::make_shared<RoundedParticleRenderable>(flatShader, particle);
        HierarchicalRenderable::addChild( systemRenderable, particleRenderable );
    }

    //RoundedParticle vs RoundedParticle collision
    {
        //Initialize two particles with position, velocity, mass and radius and add it to the system
        //One of the particle is fixed
        px = glm::vec3(0.5,0.0,0.1);
        pv = glm::vec3(0.0,0.0,0.0);
        pr = 0.1;
        pm = 1000.0;
        RoundedParticlePtr particle1 = std::make_shared<RoundedParticle>( px, pv, pm, pr);
        particle1->setFixed(true);
        system->addParticle( particle1 );

        px = glm::vec3(0.5,0.0,1.0);
        pv = glm::vec3(0.0,0.0,-0.5);
        pr = 0.1;
        pm = 1.0;
        RoundedParticlePtr particle2 = std::make_shared<RoundedParticle>( px, pv, pm, pr);
        system->addParticle( particle2 );

        //Create a particleRenderable for each particle of the system
        RoundedParticleRenderablePtr particleRenderable1 = std::make_shared<RoundedParticleRenderable>(flatShader, particle1);
        HierarchicalRenderable::addChild( systemRenderable, particleRenderable1 );
        RoundedParticleRenderablePtr particleRenderable2 = std::make_shared<RoundedParticleRenderable>(flatShader, particle2);
        HierarchicalRenderable::addChild( systemRenderable, particleRenderable2 );
    }

    //Initialize a force field that apply to all the particles of the system to simulate gravity
    //Add it to the system as a force field
    ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(system->getParticles(), glm::vec3{0,0,-10} );
    system->addForceField( gravityForceField );
}

void practical05_playPool(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable)
{
    //Initialize a shader for the following renderables
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/flatVertex.vert", "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    //Initialize a Phong shader
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/phongVertex.vert", "../shaders/phongFragment.frag"});
    viewer.addShaderProgram( phongShader );

    //Textured shader (includes phong)
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/textureVertex.vert", "../shaders/textureFragment.frag"});
    viewer.addShaderProgram( texShader );

    ShaderProgramPtr multiTexShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/multiTextureVertex.vert", "../shaders/multiTextureFragment.frag"});
    viewer.addShaderProgram( multiTexShader );


    // Materials
    
    MaterialPtr bronze = Material::Bronze();
    MaterialPtr emerald = Material::Emerald();
    MaterialPtr pearl = Material::Pearl();


    //Define a transformation
    glm::mat4 parentTransformation, localTransformation;

    // Light
    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,-1.0,-1.0));
    glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.3,0.3,0.1), d_specular(0.3,0.3,0.1);
    //glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.0,0.0,0.0), d_specular(0.0,0.0,0.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    glm::vec3 lightPosition(0.0,5.0,8.0);
    DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    directionalLightRenderable->setLocalTransform(localTransformation);
    viewer.setDirectionalLight(directionalLight);
    viewer.addRenderable(directionalLightRenderable);

    //Define a point light
    glm::vec3 p_position(0.0,0.0,0.0), p_ambient(0.0,0.0,0.0), p_diffuse(0.0,0.0,0.0), p_specular(0.0,0.0,0.0);
    float p_constant=0.0, p_linear=0.0, p_quadratic=0.0;

    p_position = glm::vec3(-8, 5.0, 5.0);
    p_ambient = glm::vec3(0.0,0.0,0.0);
    p_diffuse = glm::vec3(1.0,0.0,0.0);
    p_specular = glm::vec3(1.0,0.0,0.0);
    p_constant=1.0;
    p_linear=5e-1;
    p_quadratic=0;
    PointLightPtr pointLight1 = std::make_shared<PointLight>(p_position, p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
    PointLightRenderablePtr pointLightRenderable1 = std::make_shared<PointLightRenderable>(flatShader, pointLight1);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    pointLightRenderable1->setLocalTransform(localTransformation);
    viewer.addPointLight(pointLight1);
    viewer.addRenderable(pointLightRenderable1);

    p_position = glm::vec3(8, 5.0, 5.0);
    p_ambient = glm::vec3(0.0,0.0,0.0);
    p_diffuse = glm::vec3(0.0,0.0,1.0);
    p_specular = glm::vec3(0.0,0.0,1.0);
    p_constant=1.0;
    p_linear=5e-1;
    p_quadratic=0;
    PointLightPtr pointLight2 = std::make_shared<PointLight>(p_position, p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
    PointLightRenderablePtr pointLightRenderable2 = std::make_shared<PointLightRenderable>(flatShader, pointLight2);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    pointLightRenderable2->setLocalTransform(localTransformation);
    viewer.addPointLight(pointLight2);
    viewer.addRenderable(pointLightRenderable2);

    //Define a spot light
    glm::vec3 s_position(0.0,5.0,-8.0), s_spotDirection = glm::normalize(glm::vec3(0.0,-1.0,1.0));
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.5,0.5,0.5), s_specular(0.5,0.5,0.5);
    float s_constant=1.0, s_linear=0.0, s_quadratic=0.0;
    float s_innerCutOff=std::cos(glm::radians(20.0f)), s_outerCutOff=std::cos(glm::radians(40.0f));
    SpotLightPtr spotLight = std::make_shared<SpotLight>(s_position, s_spotDirection,
                                                         s_ambient, s_diffuse, s_specular,
                                                         s_constant, s_linear, s_quadratic,
                                                         s_innerCutOff, s_outerCutOff);
    SpotLightRenderablePtr spotLightRenderable = std::make_shared<SpotLightRenderable>(flatShader, spotLight);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    spotLightRenderable->setLocalTransform(localTransformation);
    viewer.addSpotLight(spotLight);
    viewer.addRenderable(spotLightRenderable);


    //Position the camera
    //viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0)));

    //Define materials
    glm::vec3 mAmbient(0.0), mDiffuse(0.0), mSpecular(0.0);
    float mShininess=0.0;
    MaterialPtr myMaterial = std::make_shared<Material>(mAmbient, mDiffuse, mSpecular, mShininess);

    //Initialize two particles with position, velocity, mass and radius and add it to the system
    glm::vec3 px(0.0,0.0,0.0),pv(0.0,0.0,0.0);
    float pm=1.0, pr=1.0;
    px = glm::vec3(0.0,0.0,1.0);
    RoundedParticlePtr mobile = std::make_shared<RoundedParticle>( px, pv, pm, pr);
    system->addParticle( mobile );
    px = glm::vec3(0.0,5.0,1.0);
    RoundedParticlePtr other = std::make_shared<RoundedParticle>( px, pv, pm, pr);
    system->addParticle( other );
    px = glm::vec3(0.0,8.0,1.0);
    RoundedParticlePtr other2 = std::make_shared<RoundedParticle>( px, pv, pm, pr);
    system->addParticle( other2 );
    
    //Create a particleRenderable for each particle of the system
    //Add them to the system renderable
    RoundedParticleRenderablePtr mobileRenderable = std::make_shared<RoundedParticleRenderable>( phongShader, mobile );
    mobileRenderable->setMaterial(bronze);
    HierarchicalRenderable::addChild(systemRenderable, mobileRenderable);
 

    RoundedParticleRenderablePtr otherRenderable = std::make_shared<RoundedParticleRenderable>( phongShader, other );
    otherRenderable->setMaterial(emerald);
    HierarchicalRenderable::addChild(systemRenderable, otherRenderable);
    
    RoundedParticleRenderablePtr other2Renderable = std::make_shared<RoundedParticleRenderable>( phongShader, other2 );
    other2Renderable->setMaterial(emerald);
    HierarchicalRenderable::addChild(systemRenderable, other2Renderable);
    

    //Initialize four planes to create walls arround the particles
    glm::vec3 planeNormal, planePoint;
    planeNormal = glm::vec3(-1,0,0);
    planePoint = glm::vec3(10,0,0);
    PlanePtr p0 = std::make_shared<Plane>( planeNormal, planePoint);
    system->addPlaneObstacle( p0 );

    planeNormal = glm::vec3(1,0,0);
    planePoint = glm::vec3(-10,0,0);
    PlanePtr p1 = std::make_shared<Plane>( planeNormal, planePoint);
    system->addPlaneObstacle( p1 );

    planeNormal = glm::vec3(0,-1,0);
    planePoint = glm::vec3(0,10,0);
    PlanePtr p2 = std::make_shared<Plane>( planeNormal, planePoint);
    system->addPlaneObstacle( p2 );

    planeNormal = glm::vec3(0,1,0);
    planePoint = glm::vec3(0,-10,0);
    PlanePtr p3 = std::make_shared<Plane>( planeNormal, planePoint);
    system->addPlaneObstacle( p3 );

    planeNormal = glm::vec3(0,0,1);
    planePoint = glm::vec3(0,0,0);
    PlanePtr floor = std::make_shared<Plane>( planeNormal, planePoint);
    system->addPlaneObstacle( floor );

    //Create  plane renderables to display each obstacle
    //Add them to the system renderable
    glm::vec3 x1, x2, x3, x4;
    glm::vec4 color;
    x1 = glm::vec3( 10, 10,5);
    x2 = glm::vec3( 10, 10,0);
    x3 = glm::vec3( 10,-10,0);
    x4 = glm::vec3( 10,-10,5);
    color = glm::vec4( 0.4, 0.2, 0.2, 1.0);
    PlaneRenderablePtr p1Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
    HierarchicalRenderable::addChild(systemRenderable, p1Renderable);

    x1 = glm::vec3( -10, 10,5);
    x2 = glm::vec3( -10, 10,0);
    x3 = glm::vec3( 10, 10,0);
    x4 = glm::vec3( 10, 10,5);
    color = glm::vec4( 0.4, 0.2, 0.2, 1.0);
    PlaneRenderablePtr p2Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
    HierarchicalRenderable::addChild(systemRenderable, p2Renderable);

    x1 = glm::vec3( -10, -10,5);
    x2 = glm::vec3( -10, -10,0);
    x3 = glm::vec3( -10,10,0);
    x4 = glm::vec3( -10,10,5);
    color = glm::vec4( 0.2, 0.4, 0.4, 1.0 );
    PlaneRenderablePtr p3Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
    HierarchicalRenderable::addChild(systemRenderable, p3Renderable);

    x1 = glm::vec3( 10, -10,5);
    x2 = glm::vec3( 10, -10,0);
    x3 = glm::vec3( -10,-10,0);
    x4 = glm::vec3( -10,-10,5);
    color = glm::vec4(0.2, 0.4, 0.4, 1.0);
    PlaneRenderablePtr p4Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
    HierarchicalRenderable::addChild(systemRenderable, p4Renderable);


    // Floor

    // std::string filename = "./../textures/grass_texture.png";
    // TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    // parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(10.0,10.0,10.0));
    // texPlane->setParentTransform(parentTransformation);
    // texPlane->setMaterial(pearl);
    // HierarchicalRenderable::addChild(systemRenderable, texPlane);


    //Initialize a force field that apply only to the mobile particle
    glm::vec3 nullForce(0.0,0.0,0.0);
    std::vector<ParticlePtr> vParticle;
    vParticle.push_back(mobile);
    ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vParticle, nullForce);
    system->addForceField( force );

    //Initialize a renderable for the force field applied on the mobile particle.
    //This renderable allows to modify the attribute of the force by key/mouse events
    //Add this renderable to the systemRenderable.
    ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>( flatShader, force );
    HierarchicalRenderable::addChild(systemRenderable, forceRenderable);


    //Add a damping force field to the mobile.
    DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vParticle, 0.9);
    system->addForceField( dampingForceField );

    //Activate collision and set the restitution coefficient to 1.0
    system->setCollisionsDetection(true);
    system->setRestitution(1.0f);

    //Set the camera to follow the main particle
    viewer.followParticle(mobile);
 

    //Lighted Mesh 1
    LightedMeshRenderablePtr suzanne1 = std::make_shared<LightedMeshRenderable>(phongShader, "./../meshes/suzanne.obj");
    parentTransformation = glm::translate( glm::mat4(1.0), glm::vec3(-2.0,0.0,1.0) );
    localTransformation = glm::mat4(1.0);
    suzanne1->setParentTransform(parentTransformation);
    suzanne1->setLocalTransform(localTransformation);
    suzanne1->setMaterial(bronze);
    viewer.addRenderable( suzanne1 );

    //Lighted Mesh 2
    LightedMeshRenderablePtr suzanne2 = std::make_shared<LightedMeshRenderable>(phongShader, "./../meshes/suzanne.obj");
    parentTransformation = glm::translate( glm::mat4(1.0), glm::vec3(2.0,0.0,1.0) );
    localTransformation = glm::mat4(1.0);
    suzanne2->setParentTransform(parentTransformation);
    suzanne2->setLocalTransform(localTransformation);
    suzanne2->setMaterial(emerald);
    viewer.addRenderable( suzanne2 );





}
