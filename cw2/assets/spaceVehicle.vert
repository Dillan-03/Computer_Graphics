#version 430

    layout(location = 0) uniform mat4 uProjCameraWorld;

    //Using uniform to create a random location which wil be found from the getUniformLocation
    uniform mat3 uNormalMatrix;

    layout(location = 0) in vec3 iPosition; 
    layout(location = 1) in vec3 iColor;
    layout(location = 2) in vec3 iNormal;

    out vec3 v2fColor;
    out vec3 v2fNormal;

void main() 
{ 
    // Copy input color to the output color attribute. 
    v2fColor = iColor;
    
    gl_Position = uProjCameraWorld * vec4( iPosition, 1.0 );

    v2fNormal = normalize(uNormalMatrix * iNormal);


}