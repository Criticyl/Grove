#version 460 core

in vec3 vCol;
in float vAO;
in float vVisibility;
out vec4 fragColour;

const vec3 skyColour = vec3(0.6, 0.9, 1.0);

void main()
{
    float curveAO = pow(vAO, 1.2);
    vec3 finalVertexCol = vCol * curveAO;
    
    vec3 outColour = mix(skyColour, finalVertexCol, vVisibility);
    fragColour = vec4(outColour, 1.0);
}