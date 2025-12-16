#version 460 core

in vec3 vPos;

out vec4 fragColour;

void main()
{
    fragColour = vec4(vPos + 0.5f, 1.0f);
}