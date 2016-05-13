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

in vec3 surfacePosition;
in vec4 fragmentColor;
in vec3 normal;
in vec3 cameraPosition;

in vec2 texCoord1;
in vec2 texCoord2;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform float blendingCoeff;

out vec4 outColor;

vec4 computeTextureColor(float coeff)
{
    float clampedBlendingCoeff = max(min(coeff,1.0),0.0);
    return fragmentColor * (clampedBlendingCoeff*texture2D(texSampler1, texCoord1) + (1.0-clampedBlendingCoeff)*texture2D(texSampler2, texCoord2));
}

vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Combine results
    vec3 ambient  = light.ambient * material.ambient;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 computePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance +
                                light.quadratic * (distance * distance));
    // Combine results
    vec3 ambient  = light.ambient * material.ambient;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Ambient
    vec3 ambient  = light.ambient * material.ambient;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    // Spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.spotDirection));
    float epsilon = (light.innerCutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
    //Surface to camera vector
    vec3 viewDir = normalize( cameraPosition - surfacePosition );

    int clampedNumberOfPointLight = max(0, min(numberOfPointLight, MAX_NR_POINT_LIGHTS));
    int clampedNumberOfSpotLight = max(0, min(numberOfSpotLight, MAX_NR_SPOT_LIGHTS));

    vec3 tmpColor = vec3(0.0, 0.0, 0.0);

    tmpColor += computeDirectionalLight(directionalLight, normal, viewDir);

    for(int i=0; i<clampedNumberOfPointLight; ++i)
        tmpColor += computePointLight(pointLight[i], normal, surfacePosition, viewDir);
    for(int i=0; i<clampedNumberOfSpotLight; ++i)
        tmpColor += computeSpotLight(spotLight[i], normal, surfacePosition, viewDir);

    vec4 textureColor = computeTextureColor(blendingCoeff);

    outColor = textureColor*vec4(tmpColor,1.0);
}
