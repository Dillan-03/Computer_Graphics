#version 430
in vec3 v2fColor;
in vec2 v2fTexCoord;
in vec3 v2fNormal;




//layout( location = 1 ) uniform vec3 uBaseColor;
layout( location = 0 ) out vec3 oColor;


layout( location = 2 ) uniform vec3 uLightDir; // should be normalized! kuLightDirk = 1
layout( location = 3 ) uniform vec3 uLightDiffuse; //Diffuse illumination
layout( location = 4 ) uniform vec3 uSceneAmbient; //Global ambient scene

layout ( binding = 0) uniform sampler2D uTexture;



void main()
{
    //Calculate texture
    vec3 textureColour = texture(uTexture, v2fTexCoord).rgb;

    vec3 normal = normalize(v2fNormal);

    float nDotL = max(0.0, dot(normal, uLightDir));

    oColor = (uSceneAmbient + nDotL * uLightDiffuse) * textureColour;

}