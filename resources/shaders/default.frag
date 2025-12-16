#version 460 core

in vec3 vCol;
out vec4 fragColour;

void main()
{
    fragColour = vec4(vCol, 1.0);
}