#version 430 core

    layout(location = 0) uniform mat4 uProjCameraWorld;

    layout(location = 0) in vec3 iPosition; // position
    layout(location = 1) in vec3 iColor;       // color
    layout(location = 3) in vec2 iTexCoord ;    // texture

   
    out vec3 v2fColor;
    out vec2 v2fTexCoord;

void main() 
{ 
    // Copy input color to the output color attribute. 
    v2fColor = iColor;
    
    gl_Position = uProjCameraWorld * vec4( iPosition, 1.0 );

    v2fTexCoord = iTexCoord;

}