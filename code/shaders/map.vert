///////////////// Vertex Shader

#version 400


////////// Uniforms to set

// The height map as a sampler
// (x, y, z) contains the normals
// w         contains the heights
uniform sampler2D heightMap;

// The minimal height and the scale parameter
uniform float heightMin;
uniform float heightScale;


////////// In parameters

// The position of the vertex (in world space)
// Only its (x,y) components will be used
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
    // Computing the height
    vec2 planeCoord = coord.xy;
    float height = heightMin + texture(heightMap, planeCoord).w*heightScale;

    // Returning the position with its height
    vPosition = vec3(planeCoord, height);

    // Passing the texCoords
    vTexCoord = texCoord;

}
