#version 400

uniform mat4 projMat, viewMat, modelMat;

in vec3 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTexCoord1;
in vec2 vTexCoord2;

out vec2 texCoord1;
out vec2 texCoord2;
out vec4 fragmentColor;

out vec3 normal;
out vec3 cameraPosition;
out vec3 surfacePosition;

void main()
{
    gl_Position = projMat*viewMat*modelMat*vec4(vPosition, 1.0f);
    fragmentColor = vColor;
    texCoord1 = vTexCoord1;
    texCoord2 = vTexCoord2;

    normal = normalize(transpose(inverse(mat3(modelMat))) * vNormal);
    surfacePosition = vec3(modelMat*vec4(vPosition,1.0f));
    cameraPosition = - vec3( viewMat[3] ) * mat3( viewMat );
}
