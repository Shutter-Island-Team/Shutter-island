#version 400
in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;
uniform vec3 text_color;

void main()
{    
  color = vec4(text_color.rgb, texture2D(tex, TexCoords).r);
} 
