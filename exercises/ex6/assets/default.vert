#version 430

    uniform mat4 uProjCameraWorld;
    layout( location = 0 ) in vec3 iPosition;

    layout( location = 1 ) in vec2 iTexCoord;
    // layout(location = 1) 


    // Input data 
    // The layout( location = N ) syntax allows us to specify attribute indices directly in the shader. This 
    // avoids having to call glBindAttribLocation() when creating the shader program object. See 
    // https://www.khronos.org/opengl/wiki/Layout Qualifier (GLSL) 
    // for more information. 
    // 
    // Note: the indices that we specify here must match the ones that we set up in the vertex array object. 
   
    // Output attributes 
    // Output attributes are passed from the vertex shader, interpolated across the triangle/primitive, and then 
    // passed into the fragment shader. By default, output attributes are matched by name. 
    out vec2 v2fTexCoord;
   


void main() 
{ 
    // Copy input color to the output color attribute. 
  
    
     gl_Position = uProjCameraWorld * vec4( iPosition, 1.0 );
     v2fTexCoord = iTexCoord;

     

   }