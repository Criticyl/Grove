#version 460 core

in vec3 vCol;
in float vAO;
out vec4 fragColour;

void main()
{
    vec3 colour = vCol * vAO;
    fragColour = vec4(colour, 1.0);
}