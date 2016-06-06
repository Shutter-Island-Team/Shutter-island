#version 400

uniform mat4 projMat, viewMat;
uniform mat3 NIT = mat3(1.0);

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;
layout (location = 4) in mat4 modelMat;

out vec3 fColor;

// Surfel: a SURFace ELement. All coordinates are in world space
out vec2 surfel_texCoord;
out vec3 surfel_position;
out vec3 surfel_normal;
out vec4 surfel_color;

out vec3 cameraPosition;

void main()
{
    // All attributes are in world space
	surfel_position = vec3(modelMat*vec4(position, 1.0f));
    surfel_normal = normalize( NIT * normal);
    surfel_color  = vec4(color, 1.0f);
    surfel_texCoord = texCoord;
    
    // Compute the position of the camera in world space
    cameraPosition = - vec3( viewMat[3] ) * mat3( viewMat );
    
    // Define the fragment position on the screen
    gl_Position = projMat*viewMat*vec4(surfel_position,1.0f);
}