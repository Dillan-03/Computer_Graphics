#version 430
in vec3 v2fColor; 
in vec3 v2fNormal;

layout( location = 0 ) out vec3 oColor;
layout( location = 1 ) uniform vec3 uBaseColor;


void main()
{

    oColor = v2fColor;
    vec3 normal = normalize(v2fNormal);

}
