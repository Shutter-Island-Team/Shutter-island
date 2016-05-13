// The include made here is very dirty
// Maybe we do not need them all...

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

#include "../include/Kart.hpp"

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

void initialize_world(Viewer& viewer) {

    // Initializing a dynamic solver
    DynamicSystemPtr system = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    float timeStep = 0.01;
    system->setSolver(solver);
    system->setDt(timeStep);

    // Initializing the dynamic system of the world
    DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
    viewer.addRenderable(systemRenderable);

    // Default camera position
    viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3( 0, 0, 70), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

    // Add here all the world's components

    // DEBUG : 3D Frame
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(std::list<std::string>{"../shaders/flatVertex.vert", "../shaders/flatFragment.frag"});
    viewer.addShaderProgram( flatShader );
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    // Box
    initialize_world_box(viewer, system, systemRenderable);
    // Light
    initialize_world_light(viewer, system, systemRenderable);

    // Kart
    add_Kart(viewer, system, systemRenderable);

    // Starting animation here
    viewer.startAnimation();


}


void initialize_world_box(Viewer& viewer, DynamicSystemPtr& system, 
			  DynamicSystemRenderablePtr &systemRenderable) {


    // Initializing the texture shader (includes Phong's shader)
    // Is it a bad idea to recreate a shader in each subfunction ?
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(std::list<std::string>{
	    "../shaders/textureVertex.vert",
		"../shaders/textureFragment.frag"});
    viewer.addShaderProgram(texShader);

    // Dimensions
    float x_size = 100.;
    float y_size = 100.;
    float z_size = 50.;

    // Adding planes as obstacles
    
    glm::vec3 planeNormal, planePoint;

    // Virtual walls

    planeNormal = glm::vec3(-1,0,0);
    planePoint = glm::vec3(x_size/2.,0,0);
    PlanePtr wall0 = std::make_shared<Plane>(planeNormal, planePoint);
    system->addPlaneObstacle(wall0);

    planeNormal = glm::vec3(1,0,0);
    planePoint = glm::vec3(-x_size/2.,0,0);
    PlanePtr wall1 = std::make_shared<Plane>(planeNormal, planePoint);
    system->addPlaneObstacle(wall1);

    planeNormal = glm::vec3(0,-1,0);
    planePoint = glm::vec3(0,y_size/2,0);
    PlanePtr wall2 = std::make_shared<Plane>(planeNormal, planePoint);
    system->addPlaneObstacle(wall2);

    planeNormal = glm::vec3(0,1,0);
    planePoint = glm::vec3(0,-y_size/2,0);
    PlanePtr wall3 = std::make_shared<Plane>(planeNormal, planePoint);
    system->addPlaneObstacle(wall3);

    // Virtual floor

    planeNormal = glm::vec3(0,0,1);
    planePoint = glm::vec3(0,0,0);
    PlanePtr floor = std::make_shared<Plane>(planeNormal, planePoint);
    system->addPlaneObstacle(floor);


    // Adding renderables to display them
    
    // Renderable's transformations
    glm::mat4 parentTransformation, localTransformation;
    // Material
    MaterialPtr pearl = Material::Pearl();

    // Walls
    std::string wall_filename = "./../textures/forest.jpg";
    //std::string wall_filename = "./../textures/grass_texture.png";
    

    TexturedPlaneRenderablePtr wall1_renderable = std::make_shared<TexturedPlaneRenderable>(texShader, 
											    wall_filename);
    // Scaling then putting it vertical then translating it
    glm::mat4 texturing_matrix = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0), glm::vec3(z_size, y_size, 1.0));
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0), glm::vec3(x_size/2.0, 0.0, z_size/2.0));

    parentTransformation = translation_matrix * rotation_matrix * scaling_matrix * texturing_matrix;

    wall1_renderable->setParentTransform(parentTransformation);
    wall1_renderable->setMaterial(pearl);
    HierarchicalRenderable::addChild(systemRenderable, wall1_renderable);

    
    TexturedPlaneRenderablePtr wall2_renderable = std::make_shared<TexturedPlaneRenderable>(texShader, 
											    wall_filename);
    // Scaling then putting it vertical then translating it
    translation_matrix = glm::translate(glm::mat4(1.0), glm::vec3(-x_size/2.0, 0.0, z_size/2.0));

    parentTransformation = translation_matrix * rotation_matrix * scaling_matrix * texturing_matrix;

    wall2_renderable->setParentTransform(parentTransformation);
    wall2_renderable->setMaterial(pearl);
    HierarchicalRenderable::addChild(systemRenderable, wall2_renderable);


    TexturedPlaneRenderablePtr wall3_renderable = std::make_shared<TexturedPlaneRenderable>(texShader, 
											    wall_filename);
    // Scaling then putting it vertical then translating it
    scaling_matrix = glm::scale(glm::mat4(1.0), glm::vec3(y_size, z_size, 1.0));
    rotation_matrix = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1.0, 0.0, 0.0));
    translation_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, y_size/2.0, z_size/2.0));

    parentTransformation = translation_matrix * rotation_matrix * scaling_matrix;

    wall3_renderable->setParentTransform(parentTransformation);
    wall3_renderable->setMaterial(pearl);
    HierarchicalRenderable::addChild(systemRenderable, wall3_renderable);



    TexturedPlaneRenderablePtr wall4_renderable = std::make_shared<TexturedPlaneRenderable>(texShader, 
											    wall_filename);
    // Scaling then putting it vertical then translating it
    translation_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -y_size/2.0, z_size/2.0));

    parentTransformation = translation_matrix * rotation_matrix * scaling_matrix;

    wall4_renderable->setParentTransform(parentTransformation);
    wall4_renderable->setMaterial(pearl);
    HierarchicalRenderable::addChild(systemRenderable, wall4_renderable);

	
    /*
     * Texturing the floor.
     */
    std::string floor_filename = "./../textures/grass_extended.jpeg";
    TexturedPlaneRenderablePtr floor_renderable =
        std::make_shared<TexturedPlaneRenderable>(texShader, floor_filename);
    // Scaling then centering it
    scaling_matrix = glm::scale(glm::mat4(1.0), glm::vec3(x_size, y_size, 1.0));

    parentTransformation = scaling_matrix;

    floor_renderable->setParentTransform(parentTransformation);
    floor_renderable->setMaterial(pearl);
    HierarchicalRenderable::addChild(systemRenderable, floor_renderable);

}

void initialize_world_light(
        Viewer& viewer, 
        DynamicSystemPtr& system, 
	DynamicSystemRenderablePtr &systemRenderable
) {

    // Point light for an ambiant light
    // Position
    glm::vec3 p_position(0.0, 0.0, 10.0);
    // Components
    float light_power = 40.0;
    glm::vec3 p_ambient(light_power, light_power, light_power);
    glm::vec3 p_diffuse(0.0, 0.0, 0.0);
    glm::vec3 p_specular(0.0, 0.0, 0.0);

    // Attenuation coefficients
    float p_constant = 1.0;
    float p_linear = 0.5;
    float p_quadratic = 0.0;

    // The light
    PointLightPtr pointLight1 = 
        std::make_shared<PointLight>(
            p_position,
            p_ambient,
            p_diffuse,
            p_specular,
	    p_constant, 
            p_linear, 
            p_quadratic
    );

    viewer.addPointLight(pointLight1);

}

