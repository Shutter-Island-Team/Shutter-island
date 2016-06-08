///////////////// Vertex Shader

#version 400


////////// Uniforms to set

// Camera related
uniform mat4 projMat, viewMat, modelMat;

// Position
uniform vec3 billboard_world_position;

// Dimension
uniform vec2 billboard_world_dimensions;

// Spherical -- Default : Cylindrical
uniform int spherical = 1;

////////// In parameters

// Color (Useless ?)
in vec3 vColor;

// The tex coords
in vec2 vShift;


////////// Out values

// Texture coordinate
out vec2 fTexCoord;


///////// Main

void main()
{

    // MV matrix
    mat4 mv = viewMat*modelMat;

    // First column
    mv[0][0] = 1.0;
    mv[0][1] = 0.0;
    mv[0][2] = 0.0;

    // Second column
    if (spherical == 1) {
	mv[1][0] = 0.0;
	mv[1][1] = 1.0;
	mv[1][2] = 0.0;
    }

    // Third column
    mv[2][0] = 0.0;
    mv[2][1] = 0.0;
    mv[2][2] = 1.0;

    vec3 vPosition = billboard_world_position
	+ vec3(billboard_world_dimensions*vShift, 0.0);
    gl_Position = projMat*mv*vec4(vPosition, 1.0);

    fTexCoord = vShift;

}
