#version 400

//Structure definition for Material, DirectionalLight, PointLight and SpotLight
//Parameters are exactly the same as the corresponding C++ classes
//Refer to the C++ documentation for more information

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 spotDirection;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float innerCutOff;
    float outerCutOff;
};

uniform Material material;
uniform DirectionalLight directionalLight;

#define MAX_NR_POINT_LIGHTS 10
uniform int numberOfPointLight;
uniform PointLight pointLight[MAX_NR_POINT_LIGHTS];

#define MAX_NR_SPOT_LIGHTS 10
uniform int numberOfSpotLight;
uniform SpotLight spotLight[MAX_NR_SPOT_LIGHTS];

// Surfel: a SURFace ELement. All coordinates are in world space
in vec3 surfel_position;
in vec4 surfel_color;
in vec3 surfel_normal;

// Camera position in world space
in vec3 cameraPosition;

// Resulting color of the fragment shader
out vec4 outColor;

//Phong illumination model for a directional light
vec3 computeDirectionalLight(DirectionalLight light, vec3 surfel_to_camera)
{
    vec3 surfel_to_light = -light.direction;

    // Diffuse shading
    float diffuse_factor = max(dot(surfel_normal, surfel_to_light), 0.0);

    // Specular shading
    vec3 reflect_direction = reflect(-surfel_to_light, surfel_normal);
    float specular_factor = pow(max(dot(surfel_to_camera, reflect_direction), 0.0), material.shininess);

    // Combine results    
    vec3 ambient  =                   light.ambient  * material.ambient  * vec3(surfel_color);
    vec3 diffuse  = diffuse_factor  * light.diffuse  * material.diffuse  * vec3(surfel_color);
    vec3 specular = specular_factor * light.specular * material.specular * vec3(surfel_color);


    return (ambient + diffuse + specular);
}

//Phong illumination model for a point light
vec3 computePointLight(PointLight light, vec3 surfel_to_camera)
{
    // Diffuse shading
    vec3 surfel_to_light = light.position - surfel_position;
    float distance = length( surfel_to_light );
    surfel_to_light *= float(1) / distance;
    float diffuse_factor = max(dot(surfel_normal, surfel_to_light), 0.0);
    
    // Specular shading
    vec3 reflect_direction = reflect(-surfel_to_light, surfel_normal);
    float specular_factor = pow(max(dot(surfel_to_camera, reflect_direction), 0.0), material.shininess);

    // Attenuation
    float attenuation = 1.0/(light.constant + light.linear*distance + light.quadratic*distance*distance);

     // Combine results    
    vec3 ambient  = attenuation *                   light.ambient  * material.ambient  * vec3(surfel_color);
    vec3 diffuse  = attenuation * diffuse_factor  * light.diffuse  * material.diffuse  * vec3(surfel_color);
    vec3 specular = attenuation * specular_factor * light.specular * material.specular * vec3(surfel_color);

    return (ambient + diffuse + specular);
}

//Phong illumination model for a spot light
vec3 computeSpotLight(SpotLight light, vec3 surfel_to_camera)
{
    
    vec3 surfel_to_light = light.position - surfel_position;
		
    // Diffuse
    float distance = length( surfel_to_light );
    surfel_to_light *= float(1) / distance;
    float diffuse_factor = max(dot(surfel_normal, surfel_to_light), 0.0);
    


    // Specular
    vec3 reflect_direction = reflect(-surfel_to_light, surfel_normal);
    float specular_factor = pow(max(dot(surfel_to_camera, reflect_direction), 0.0), material.shininess);

    float intensity;
    // Lighting cone
    float posCutOff = -dot(surfel_to_light, light.spotDirection);
    if (posCutOff < light.outerCutOff)
       intensity = 0.0;
    else
	// Spotlight (soft edges)
    	intensity = max(0.0, min(1.0, (posCutOff - light.outerCutOff)/(light.innerCutOff - light.outerCutOff)));
		    
    // Attenuation
    float attenuation = 1.0/(light.constant + light.linear*distance + light.quadratic*distance*distance);;

    // Combine results
    float coefficient = intensity * attenuation;
    vec3 ambient  = coefficient *                  light.ambient  * material.ambient  * vec3(surfel_color);
    vec3 diffuse  = coefficient * diffuse_factor  * light.diffuse  * material.diffuse  * vec3(surfel_color);
    vec3 specular = coefficient * specular_factor * light.specular * material.specular * vec3(surfel_color);

    return (ambient + diffuse + specular);
}

void main()
{
    //Surface to camera vector
    vec3 surfel_to_camera = normalize( cameraPosition - surfel_position );

    int clampedNumberOfPointLight = max(0, min(numberOfPointLight, MAX_NR_POINT_LIGHTS));
    int clampedNumberOfSpotLight = max(0, min(numberOfSpotLight, MAX_NR_SPOT_LIGHTS));

    vec3 tmpColor = vec3(0.0, 0.0, 0.0);

    tmpColor += computeDirectionalLight(directionalLight, surfel_to_camera);

    for(int i=0; i<clampedNumberOfPointLight; ++i)
        tmpColor += computePointLight(pointLight[i], surfel_to_camera);

    for(int i=0; i<clampedNumberOfSpotLight; ++i)
        tmpColor += computeSpotLight(spotLight[i], surfel_to_camera);

    outColor = vec4(tmpColor,1.0);
}
