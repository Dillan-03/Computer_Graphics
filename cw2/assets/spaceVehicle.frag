#version 430

// input variables
in vec3 v2fColor;
in vec3 v2fNormal;
in vec3 v2fPosition;

// assigning location variables for when it is being called from the main and can be further used

// layout( location = 2 ) uniform vec3 uLightDir; //normalized 
// layout( location = 3 ) uniform vec3 uLightDiffuse; //Diffuse illumination
// layout( location = 4 ) uniform vec3 uSceneAmbient; //Global ambient scene
// layout( location = 5 ) uniform vec3 uLightSpecular; // Specular illumination
// layout( location = 6 ) uniform float uShininess; // Shininess factor for specular


//Variables for the three positions and colours of the light
layout (location = 2) uniform vec3 uLightPositions; // Positions of three lights
layout (location = 3) uniform vec3 uLightColors; // Colors of three lights
layout( location = 4 ) uniform vec3 uCameraPos; // Camera position for specular calculation

// output the lighting on the space vehicle
layout( location = 0 ) out vec3 oColor;

void main()
{

    //Normalise the normal so that the space vehicle can be lit correctly 
    vec3 normal = normalize(v2fNormal);

    vec3 temp = vec3(0.0); // 

    vec3 uSceneAmbient = vec3(0.1, 0.3, 0.1);

    // Method of Blinn-Phong is used from the following [https://learnopengl.com/Advanced-Lighting/Advanced-Lighting]
    
    // Normalise the lighting calculation to create a real effect
    vec3 lightCalculated = uLightPositions;
    float distance = length(lightCalculated);
    vec3 uLightDir = normalize(lightCalculated);

    // Ambient component
    vec3 ambient = uSceneAmbient * v2fColor;

    // Diffuse component
    float diff = max(dot(normal, uLightDir), 0.0);

    // Diffuse variable which creates a a lightning reflecting effect. 
    // On a darker colours and when the light is shown on it directly, it will shine brigher
    vec3 diffuse = uLightColors * diff * v2fColor;

    // Specular component

    vec3 viewDir = normalize(uCameraPos - v2fPosition);
    vec3 halfwayDir = normalize(uLightDir + viewDir);

    // float UShininess = 60.0; // Example value, adjust as needed

    float spec = pow(max(dot(normal, halfwayDir), 0.0), 80.0);
    // vec3 specular = uLightSpecular * spec * v2fColor;
    vec3 specular = uLightColors * spec; // Constant specular for testing

    // Distance attenuation
    float attenuation = 1.0 / (distance * distance);

    // Combining components
    oColor = (ambient + (diffuse + specular) * attenuation);

  
}

