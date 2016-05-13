#version 400

uniform mat4 projMat, viewMat, modelMat;

// This is the normal inverse transpose matrix.
// It is really important to obtain a normal in world coordinates.
// It should NEVER BE COMPUTED IN THE VERTEX SHADER, since an inverse computation
// is quite expensive and the result is the same for all vertices.
uniform mat3 NIT = mat3(1.0);

// Attributes
in vec2 vTexCoord;
in vec3 vPosition;
in vec4 vColor;   // Currently not used. You can use it to replace or combine with the diffuse component of the material
in vec3 vNormal;

// Surfel: a SURFace ELement. All coordinates are in world space
out vec2 surfel_texCoord;
out vec3 surfel_position;
out vec3 surfel_normal;
out vec4 surfel_color;

out vec3 cameraPosition;

void main()
{
    // All attributes are in world space
    surfel_position = vec3(modelMat*vec4(vPosition,1.0f));
    surfel_normal = normalize( NIT * vNormal);
    surfel_color  = vColor;
    surfel_texCoord = vTexCoord;

    // Compute the position of the camera in world space
    cameraPosition = - vec3( viewMat[3] ) * mat3( viewMat );

    // Define the fragment position on the screen
    gl_Position = projMat*viewMat*vec4(surfel_position,1.0f);
}
