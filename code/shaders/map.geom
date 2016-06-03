//////////////////// Geometry Shader

#version 400


// Handles triangles
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;


////////// Uniforms to set

// The height map as a sampler
// (x, y, z) contains the normals - used later
// w         contains the heights
uniform sampler2D heightMap;

////////// In parameters

// The texture coordinates
in vec2 tsTexCoord[3];

////////// Out values

// The texture coordinates
out vec2 gTexCoord;

// The associated normal
out vec3 gNormal;

////////// Function to 'unnormalize' normals

vec3 restoreNormal(vec3 normal) 
{
    return 2*(normal - vec3(0.5, 0.5, 0.5));
}

///////// Main

void main() {

    // Vertex 0
    // gl_Position
    gl_Position = gl_in[0].gl_Position;
    // Normal
    gNormal = restoreNormal(texture(heightMap, tsTexCoord[0]).xyz);
    // TexCoord
    gTexCoord = tsTexCoord[0];
    EmitVertex();
    
    // Vertex 1
    // gl_Position
    gl_Position = gl_in[1].gl_Position;
    // Normal
    gNormal = restoreNormal(texture(heightMap, tsTexCoord[1]).xyz);
    // TexCoord
    gTexCoord = tsTexCoord[1];
    EmitVertex();

    // Vertex 2
    // gl_Position
    gl_Position = gl_in[2].gl_Position;
    // Normal
    gNormal = restoreNormal(texture(heightMap, tsTexCoord[2]).xyz);
    // TexCoord
    gTexCoord = tsTexCoord[2];
    EmitVertex();

    EndPrimitive();
}

