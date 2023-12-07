#version 430
in vec3 v2fColor;
in vec3 v2fNormal;
in vec3 v2fPosition;
//layout( location = 1 ) uniform vec3 uBaseColor;

layout( location = 2 ) uniform vec3 uLightDir; //normalized 
layout( location = 3 ) uniform vec3 uLightDiffuse; //Diffuse illumination
layout( location = 4 ) uniform vec3 uSceneAmbient; //Global ambient scene
layout( location = 5 ) uniform vec3 uLightSpecular; // Specular illumination
layout( location = 6 ) uniform float uShininess; // Shininess factor for specular
layout( location = 7 ) uniform vec3 uCameraPos; // Camera position for specular calculation

// Activate the lighting on the space vehicle
layout( location = 0 ) out vec3 oColor;

void main()
{

    vec3 normal = normalize(v2fNormal);

    vec3 lightCalculated = uLightDir;
    float distance = length(lightCalculated);
    // Ensure the light direction is normalized
    vec3 uLightDir = normalize(lightCalculated);

    // Ambient component
    vec3 ambient = uSceneAmbient * v2fColor;

    // Diffuse component
    float diff = max(dot(normal, uLightDir), 0.0);
    vec3 diffuse = uLightDiffuse * diff * v2fColor;

    // Specular component
    vec3 viewDir = normalize(uCameraPos - v2fPosition);
    vec3 halfwayDir = normalize(uLightDir + viewDir);

    float UShininess = 60.0; // Example value, adjust as needed
    float spec = pow(max(dot(normal, halfwayDir), 0.0), uShininess);
    // vec3 specular = uLightSpecular * spec * v2fColor;
    // Temporarily remove specular component to test
    vec3 specular = vec3(0.1f); // Constant specular for testing

    // Distance attenuation
    float attenuation = 1.0 / (distance * distance);

    // Combining components
    oColor = (ambient + (diffuse + specular) * attenuation);
    // oColor = vec3(1.0, 0.0, 0.0);

  
}

