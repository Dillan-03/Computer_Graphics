#version 430


    // Input data 
    // The layout( location = N ) syntax allows us to specify attribute indices directly in the shader. This 
    // avoids having to call glBindAttribLocation() when creating the shader program object. See 
    // https://www.khronos.org/opengl/wiki/Layout Qualifier (GLSL) 
    // for more information. 
    // 
    // Note: the indices that we specify here must match the ones that we set up in the vertex array object. 
    layout( location = 0 ) in vec2 iPosition; 
    layout( location = 1 ) in vec3 iColor; 

    // Output attributes 
    // Output attributes are passed from the vertex shader, interpolated across the triangle/primitive, and then 
    // passed into the fragment shader. By default, output attributes are matched by name. 
    out vec3 v2fColor; // v2f = vertex to fragment

void main() 
{ 
    // Copy input color to the output color attribute. 
    v2fColor = iColor; 
    // Copy position to the built-in gl Position attribute 
    // This attribute is the clip space position (homogeneous vertex position), which is always a vec4. For 
    // now, we set z and w to zero and one, respectively. (z = zero is in the middle of the OpenGL clip space. 
    // w = one, since this is a point and not a direction.) 
    gl_Position = vec4( iPosition.xy, 0.0, 1.0 ); 
}
