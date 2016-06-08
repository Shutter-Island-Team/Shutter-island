//////////////////// Fragment Shader

#version 400


////////// Uniforms to set

// The texture
uniform sampler2D texSampler;


////////// In parameters

// Texture coordinate
in vec2 fTexCoord;


////////// Out values

// Color
out vec4 outColor;


////////// Main

void main() {
    outColor = texture(texSampler, fTexCoord);
}
