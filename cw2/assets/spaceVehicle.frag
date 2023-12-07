#version 430
in vec3 v2fColor;
in vec3 v2fNormal;

//layout( location = 1 ) uniform vec3 uBaseColor;
layout( location = 0 ) out vec3 oColor;

layout( location = 2 ) uniform vec3 uLightDir; // should be normalized LightDir = 1
layout( location = 3 ) uniform vec3 uLightDiffuse; //Diffuse illumination
layout( location = 4 ) uniform vec3 uSceneAmbient; //Global ambient scene

// Activate the lighting on the space vehicle

void main()
{
    vec3 normal = normalize(v2fNormal);


    // Ensure the light direction is normalized
    vec3 lightDir = normalize(uLightDir); 

    // Ambient component
    vec3 ambient = uSceneAmbient * v2fColor;

    // Diffuse component
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = uLightDiffuse * diff * v2fColor;

    // Combining ambient and diffuse components
    oColor = ambient + diffuse;
  
}
 
