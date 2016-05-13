#version 400

//Design choice : Color are always vec4
in vec4 fragmentColor;
out vec4 outColor;

void main()
{
    outColor = fragmentColor;
}
