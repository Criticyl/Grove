#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in float aAO;

out vec3 vCol;
out float vAO;
out float vVisibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float fogDensity = 0.02;
const float fogGradient = 1.5;

void main()
{
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    gl_Position = projection * viewPos;
    vCol = aCol;
    vAO = aAO;

    float dist = length(viewPos.xyz);
    vVisibility = exp(-pow((dist * fogDensity), fogGradient));
    vVisibility = clamp(vVisibility, 0.0, 1.0);
}