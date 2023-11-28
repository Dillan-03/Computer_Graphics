#version 430
// Input attributes 
// These should match the outputs from the vertex shader. 

in vec3 v2fColor; 

// Uniform data 
// For now, we use the “register style” uniforms. Similar to the vertex shader inputs, the 
// layout( location = N ) 
// syntax allows us to specify uniform locations in the shader (which avoids the need to query them later, 
// via glGetUniformLocation()). Again, see 
// https://www.khronos.org/opengl/wiki/Layout Qualifier (GLSL) 
// for more information 
// 
// Warning: This feature was added in OpenGL 4.3 / GLSL 430! (Apple: check if Apple supports the 
// GL ARB explicit uniform location extension, which provides the same functionality.) 

layout( location = 0 ) uniform vec3 uBaseColor;

// Fragment shader outputs 
// For now, we have a single output, a RGB (=vec3) color. This is written to the color attachment with 
// index 0 (the location value below). For the moment, we are drawing the default framebuffer, which has 
// only one color attachment: color attachment 0, which is the window’s back buffer. 
layout( location = 0 ) out vec3 oColor;

void main()
{
    oColor = uBaseColor * v2fColor;
}
