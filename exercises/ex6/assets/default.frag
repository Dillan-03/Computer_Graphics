#version 430
// Input attributes 
// These should match the outputs from the vertex shader. 
// 
in vec2 v2fTexCoord;


layout ( binding = 0) uniform sampler2D uTexture;


out vec3 oColor;

void main()
{
    oColor = texture(uTexture, v2fTexCoord).rgb;
   

}