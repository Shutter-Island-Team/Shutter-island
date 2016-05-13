#include "../include/world.hpp"

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

#include "../include/dynamics/KartCompartmentRenderable.hpp"
#include "../include/dynamics/KartChassisRenderable.hpp"
#include "../include/dynamics/KartWheelRenderable.hpp"

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




void add_Kart(Viewer& viewer, DynamicSystemPtr& system, 
	      DynamicSystemRenderablePtr &systemRenderable) {

    //Initialize a shader for the following renderables
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/flatVertex.vert", "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );

    //Initialize a Phong shader
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/phongVertex.vert", "../shaders/phongFragment.frag"});
    viewer.addShaderProgram( phongShader );

    // Material
    MaterialPtr pearl = Material::Pearl();

    //Define a transformation
    glm::mat4 parentTransformation, localTransformation;

    glm::vec3 px, pv;
    float pm, pr;

    // TODO : Replace here RoundedParticles by CynlinderParticles (to be created, no collision needed)
    // Front right wheel
    px = glm::vec3(3.0, 1.0, 1.0);
    pv = glm::vec3(0.0, 0.0, 0.0);
    pm = 1.0; pr = 1.0;
    RoundedParticlePtr kartWheel1 = std::make_shared<RoundedParticle>( px, pv, pm, pr);
    kartWheel1->setCollisionable(false);
    system->addParticle(kartWheel1);
    KartWheelRenderablePtr kartWheel1Renderable = std::make_shared<KartWheelRenderable>(phongShader, kartWheel1, 50);
    kartWheel1Renderable->setMaterial(pearl);
    
    
    // Front left wheel
    px = glm::vec3(3.0, -1.0, 1.0);
    RoundedParticlePtr kartWheel2 = std::make_shared<RoundedParticle>( px, pv, pm, pr);
    kartWheel2->setCollisionable(false);
    system->addParticle(kartWheel2);
    KartWheelRenderablePtr kartWheel2Renderable = std::make_shared<KartWheelRenderable>(phongShader, kartWheel2, 50);    
    kartWheel2Renderable->setMaterial(pearl);

    // Back right wheel
    px = glm::vec3(-3.0, 1.0, 1.0);
    RoundedParticlePtr kartWheel3 = std::make_shared<RoundedParticle>( px, pv, pm, pr);
    kartWheel3->setCollisionable(false);
    kartWheel3->setFixed(true);
    system->addParticle(kartWheel3);
    KartWheelRenderablePtr kartWheel3Renderable = std::make_shared<KartWheelRenderable>(phongShader, kartWheel3, 50);
    kartWheel3Renderable->setMaterial(pearl);

    // Back left wheel
    px = glm::vec3(-3.0,-1.0, 1.0);
    RoundedParticlePtr kartWheel4 = std::make_shared<RoundedParticle>( px, pv, pm, pr);
    kartWheel4->setCollisionable(false);
    kartWheel4->setFixed(true);
    system->addParticle(kartWheel4);
    KartWheelRenderablePtr kartWheel4Renderable = std::make_shared<KartWheelRenderable>(phongShader, kartWheel4, 50); 
    kartWheel4Renderable->setMaterial(pearl);

    // The two chassis to link the wheels
    // pm not used here
    // Front
    px = glm::vec3(3.0, 0., 1.0);
    StickParticlePtr kartFrontChassis = std::make_shared<StickParticle>(px, pv, pm, kartWheel1, kartWheel2);
    kartFrontChassis->setCollisionable(false);
    system->addParticle(kartFrontChassis);
    KartChassisRenderablePtr kartFrontChassisRenderable = std::make_shared<KartChassisRenderable>(phongShader, kartFrontChassis);
    kartFrontChassisRenderable->setMaterial(pearl);
	
    // Back
    px = glm::vec3(-3.0, 0., 1.0);
    StickParticlePtr kartBackChassis = std::make_shared<StickParticle>(px, pv, pm, kartWheel3, kartWheel4);
    kartBackChassis->setCollisionable(false);
    system->addParticle(kartBackChassis);
    KartChassisRenderablePtr kartBackChassisRenderable = std::make_shared<KartChassisRenderable>(phongShader, kartBackChassis);
    kartBackChassisRenderable->setMaterial(pearl);

    // Adding now the wheels in the hierarchy as chassis are now defined
    HierarchicalRenderable::addChild(kartFrontChassisRenderable, kartWheel1Renderable);
    HierarchicalRenderable::addChild(kartFrontChassisRenderable, kartWheel2Renderable);
    HierarchicalRenderable::addChild(kartBackChassisRenderable, kartWheel3Renderable);
    HierarchicalRenderable::addChild(kartBackChassisRenderable, kartWheel4Renderable);

 
    // Kart
    px = glm::vec3(0.0,0.0,1.0); 
    pv = glm::vec3(0.0,0.0,0.0);
    float dx = 2.0;
    float dy = 5.0;
    float dz = 1.0;
    pm = 1.0; 
    pr = 1.0;
    BoxParticlePtr kart = std::make_shared<BoxParticle>(px, pv, pm, dx, dy, dz, kartFrontChassis, kartBackChassis);
    kart->setFixed(true);
    //RoundedParticlePtr kart = std::make_shared<RoundedParticle>( px, pv, pm, pr);
    system->addParticle( kart );
    KartCompartmentRenderablePtr kartCompartmentRenderable = std::make_shared<KartCompartmentRenderable>( phongShader, kart );
    kartCompartmentRenderable->setMaterial(pearl);
    HierarchicalRenderable::addChild(systemRenderable, kartCompartmentRenderable);

    // Adding now the chassis in the herarchy as the compartment is defined 
    HierarchicalRenderable::addChild(kartCompartmentRenderable, kartBackChassisRenderable);
    HierarchicalRenderable::addChild(kartCompartmentRenderable, kartFrontChassisRenderable);

    // Control definition
    // The two front wheels control the kart
    // The back of the kart follows the hierarchy
    // (Default control : both children control the parent)
    kart->setControlChild2(true);
    kartBackChassis->setControlChildren(true);

    // Also tells the kart that particle is the main particle of a hierarchy
    ParticlePtr kartPtr = (ParticlePtr) std::static_pointer_cast<Particle, BoxParticle>(kart);
    kartCompartmentRenderable->setMasterParticle(kartPtr);

    // Finally tells the viewer what it should follow
    viewer.followParticle(kart);
    

    //Initialize a force field that apply only to the mobile particle
    glm::vec3 nullForce(0.0,0.0,0.0);
    std::vector<ParticlePtr> vParticle;
    vParticle.push_back(kartWheel1);
    vParticle.push_back(kartWheel2);
    // Kart added only to set the camera
    vParticle.push_back(kart);
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

}
