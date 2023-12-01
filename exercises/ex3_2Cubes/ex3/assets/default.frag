#version 430

//input attributes 
//These shoild match the output from the vertex shader

in vec3 v2fColor;



//layout( location = 1) uniform vec3 uBaseColor;

//Fragment shader output 

layout (location = 0 ) out vec3 oColor;

void main()
{
    oColor = v2fColor;
}