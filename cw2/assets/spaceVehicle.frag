#version 430

// input variables
in vec3 v2fColor;
in vec3 v2fNormal;
in vec3 v2fPosition;


// Assigning location variables for when it is being called from the main and can be further used in shader program

// Location 2: Position of the first light source in the scene
layout( location = 2 ) uniform vec3 uLightPositionOne;

// Location 3: Colour of the first light source
layout( location = 3 ) uniform vec3 uLightColourOne; 

// Location 4: Position of the camera for the first viewpoint
layout(location = 4) uniform vec3 uCameraPosOne; 

// Location 5: Position of the second light source in the scene
layout( location = 5 ) uniform vec3 uLightPositionTwo;

// Location 6: Colour of the second light source
layout( location = 6 ) uniform vec3 uLightColourTwo;

// Location 7: Position of the camera for the second viewpoint
layout(location = 7) uniform vec3 uCameraPosTwo; 

// Location 8: Position of the third light source in the scene
layout( location = 8 ) uniform vec3 uLightPositionThree; 

// Location 9: Colour of the third light source
layout( location = 9 ) uniform vec3 uLightColourThree; 

// Location 10: Position of the camera for the third viewpoint
layout(location = 10 ) uniform vec3 uCameraPosThree; 

// Location 11: Ambient colour of the scene, affecting all objects uniformly
layout(location = 11 ) uniform vec3 uSceneAmbient; 


// Creating a ist to hold the corresponding light values

// output the lighting on the space vehicle
layout( location = 0 ) out vec3 oColor;

void main()
{
    //Normalise the normal so that the space vehicle can be lit correctly
    //Normalise the lighting calculation to create a real effect

    vec3 normal = normalize(v2fNormal);

    // vec3 uSceneAmbient 

    float UShininess = 16.0; //how shiny the object will be

    oColor = vec3(0.0); // Initialize oColor to prevent using uninitialized value

    // Process each light
    // Initialising an array of light positions, each uLightPosition represents the position of a light source in the scene
    vec3 uLightPosition[3] = {uLightPositionOne, uLightPositionTwo, uLightPositionThree};

    // Initialising an array of light colours, each uLightColour represents the colour of a light source in the scene
    vec3 uLightColour[3] = {uLightColourOne, uLightColourTwo, uLightColourThree};

    // Initialising an array of camera positions, each uCameraPos represents the camera position of a light source in the scene
    vec3 uCameraPos[3] = {uCameraPosOne, uCameraPosTwo, uCameraPosThree};

    // Method of Blinn-Phong is used from the following [https://learnopengl.com/Advanced-Lighting/Advanced-Lighting]
    // Loop to process each light source 
    for (int light = 0; light < 3; ++light) {
        // Ambient component representing the scene's ambient light colour
        vec3 ambient = uSceneAmbient * 0.5 * v2fColor;

        // Diffuse component, the dot product of the normalised surface normal and light direction (which determines how much light hits the surface)
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
        // Inverse square law 
        float distance = length(uLightPosition[light] - v2fPosition);
        float attenuation = 1.0 / (distance * distance);

        // Combining components
        oColor += (ambient + (diffuse + specular) * attenuation);

    }

}