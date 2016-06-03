//////////////////// Fragment Shader

#version 400


////////// Lights

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


////////// Uniforms to set

///// Textures
uniform sampler2D seaTex;
uniform sampler2D sandTex;
uniform sampler2D plainsTex;
uniform sampler2D lakeTex;
uniform sampler2D mountainTex;

///// Masks 
uniform sampler2D seaMask;
uniform sampler2D sandMask;
uniform sampler2D plainsMask;
uniform sampler2D lakeMask;
uniform sampler2D mountainMask;

////////// In parameters

// The texture coordinates
in vec2 gTexCoord;

// The associated normal
in vec3 gNormal;


////////// Out values

out vec4 fColor;


///////// Illumination functions

// Phong illumination model for a directional light
vec3 computeDirectionalLight(DirectionalLight light, vec3 positionToCamera)
{
    vec3 positionToLight = -light.direction;

    // Diffuse shading
    float diffuseFactor = max(dot(gNormal, positionToLight), 0.0);

    // Specular shading
    vec3 reflectDirection = reflect(-positionToLight, gNormal);
    float specularFactor = pow(max(dot(positionToCamera, reflectDirection), 0.0), material.shininess);

    // Combine results
    vec3 ambient  =                  light.ambient  * material.ambient ;
    vec3 diffuse  = diffuseFactor  * light.diffuse  * material.diffuse ;
    vec3 specular = specularFactor * light.specular * material.specular;

    return (ambient + diffuse + specular);
}

// Phong illumination model for a point light
vec3 computePointLight(PointLight light, vec3 positionToCamera)
{
    // Diffuse shading
    vec3 positionToLight = light.position - positionToCamera;
    float distance = length(positionToCamera);
    positionToLight *= float(1) / distance;
    float diffuseFactor = max(dot(gNormal, positionToLight), 0.0);

    // Specular shading
    vec3 reflectDirection = reflect(-positionToLight, gNormal);
    float specularFactor = pow(max(dot(positionToCamera, reflectDirection), 0.0), material.shininess);

    // Attenuation:
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine results
    vec3 ambient  = attenuation *                  light.ambient  * material.ambient ;
    vec3 diffuse  = attenuation * diffuseFactor  * light.diffuse  * material.diffuse ;
    vec3 specular = attenuation * specularFactor * light.specular * material.specular;

    return (ambient + diffuse + specular);
}

// Phong illumination model for a spot light
vec3 computeSpotLight(SpotLight light, vec3 positionToCamera)
{
    // Diffuse
    vec3 positionToLight = light.position - positionToCamera;
    float distance = length(positionToLight);
    positionToLight *= float(1) / distance;
    float diffuseFactor = max(dot(gNormal, positionToLight), 0.0);

    // Specular
    vec3 reflectDirection = reflect(-positionToLight, gNormal);
    float specularFactor = pow(max(dot(positionToCamera, reflectDirection), 0.0), material.shininess);

    // Spotlight (soft edges): 
    float cos_theta = dot(positionToLight, -light.spotDirection);
    float intensity = clamp( (cos_theta - light.outerCutOff ) / ( light.innerCutOff - light.outerCutOff ), 0, 1 );

    // Attenuation
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine results
    vec3 ambient  =             attenuation *                  light.ambient  * material.ambient ;
    vec3 diffuse  = intensity * attenuation * diffuseFactor  * light.diffuse  * material.diffuse ;
    vec3 specular = intensity * attenuation * specularFactor * light.specular * material.specular;

    return (ambient + diffuse + specular);
}

///////// Main

void main() {

    // Compute the light and blend the texture
    //fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);   
    fColor = vec4((gNormal + vec3(1.0f))/2, 1.0f);   
}
