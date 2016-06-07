///////////////// Vertex Shader

#version 400


////////// Uniforms to set

// Matrices
uniform mat4 projMat, viewMat, modelMat;

////////// In parameters

// The position of the vertex (in world space)
// Only its (x,y) components will be used
in vec3 coord;

////////// Out values

// The position with its height
out vec3 vPosition;


////////// Main

void main()
{


    // Passing the position
    vPosition = coord;

    // GlPosition
    gl_Position = projMat*viewMat*modelMat*vec4(vPosition, 1.0f);
}
