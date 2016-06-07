///////////////// Vertex Shader

#version 400


////////// Uniforms to set


////////// In parameters

// The position of the vertex (in world space) with its height
in vec3 coord;

// The texture coordinates
in vec2 texCoord;

////////// Out values

// The position with its height
out vec3 vPosition;

// The textures coordinates
out vec2 vTexCoord;


////////// Main

void main()
{

    // Passing the position
    vPosition = coord;

    // Passing the texCoords
    vTexCoord = texCoord;

}
