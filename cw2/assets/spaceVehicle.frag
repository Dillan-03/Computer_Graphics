#version 430

// input variables
in vec3 v2fColor;
in vec3 v2fNormal;
in vec3 v2fPosition;


// assigning location variables for when it is being called from the main and can be further used
layout( location = 2 ) uniform vec3 uLightPositionOne; //normalized (positional light)
layout( location = 3 ) uniform vec3 uLightColourOne; //Diffuse illumination
layout(location = 4) uniform vec3 uCameraPosOne; // Camera position for specular calculation
layout( location = 5 ) uniform vec3 uLightPositionTwo;//normalized (positional light)
layout( location = 6 ) uniform vec3 uLightColourTwo; //Diffuse illumination
layout(location = 7) uniform vec3 uCameraPosTwo; // Camera position for specular calculation
layout( location = 8 ) uniform vec3 uLightPositionThree; //normalized (positional light)
layout( location = 9 ) uniform vec3 uLightColourThree; //Diffuse illumination
layout(location = 10 ) uniform vec3 uCameraPosThree; // Camera position for specular calculation
layout(location = 11 ) uniform vec3 uSceneAmbient; // Camera position for scene ambient


// Creating a ist to hold the corresponding light values

// output the lighting on the space vehicle
layout( location = 0 ) out vec3 oColor;

void main()
{
    //Normalise the normal so that the space vehicle can be lit correctly
    //Normalise the lighting calculation to create a real effect

    vec3 normal = normalize(v2fNormal);

    // vec3 uSceneAmbient 

    float UShininess = 32.0; //how shiny the object will be

    oColor = vec3(0.0); // Initialize oColor to prevent using uninitialized value

    // Process each light
    vec3 uLightPosition[3] = {uLightPositionOne, uLightPositionTwo, uLightPositionThree};
    vec3 uLightColour[3] = {uLightColourOne, uLightColourTwo, uLightColourThree};
    vec3 uCameraPos[3] = {uCameraPosOne, uCameraPosTwo, uCameraPosThree};

   // Method of Blinn-Phong is used from the following [https://learnopengl.com/Advanced-Lighting/Advanced-Lighting]
    for (int light = 0; light < 3; ++light) {
        // Ambient component
        vec3 ambient = uSceneAmbient * 0.5 * v2fColor;

        // Diffuse component
        float diff = max(dot(normal, uLightPosition[light]), 0.0);

        // Diffuse variable which creates a a lightning reflecting effect. 
        // On a darker colours and when the light is shown on it directly, it will shine brigher
        vec3 diffuse = uLightColour[light] * diff * v2fColor;

        // Specular component
        vec3 viewDir = normalize(uCameraPos[light] - v2fPosition);
        vec3 halfwayDir = normalize(uLightPosition[light] + viewDir);

    
        float spec = pow(max(dot(normal, halfwayDir), 0.0), UShininess);
        vec3 specular = uLightColour[light] * spec;

        // Distance attenuation
        // inverse square law 
        float distance = length(uLightPosition[light] - v2fPosition);
        float attenuation = 1.0 / (distance * distance);

        // Combining components
        oColor += (ambient + (diffuse + specular) * attenuation);

    }

    // oColor = oColor;

}