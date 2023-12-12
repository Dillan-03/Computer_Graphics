#version 440
layout(location = 0) uniform mat4 uProjCameraWorld;

in vec3 iPosition; 
in vec3 iColor;

layout( location = 0 ) out vec3 oColor;

void main() {
    gl_Position = uProjCameraWorld * vec4(iPosition, 1.0);
    oColor = iColor;

}
