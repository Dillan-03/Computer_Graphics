#version 430

layout ( location = 0 ) in vec3 iPosition;
layout (location =1 ) in vec3 iColor;
//layout (location = 0 ) uniform mat2 uRotation;
layout(location = 0 ) uniform mat4 uProjCameraWorld;

//output atributes 
//output attributes are passedfrom the vertex shader, interpolated across the triangle/ primitive, and then 
//passed into the fragment shader. By default, output attributes are mathced by name,
 out vec3 v2fColor; // v2f = vertex to fragment

//Each shader has a main method 
void main()
{
    //Copy the color to the output color attribute.

    v2fColor = iColor;

    //
    //
    
    gl_Position = uProjCameraWorld * vec4( iPosition, 1.0);

   
}

