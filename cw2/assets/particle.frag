// #version 430
// in vec3 v2fColor;

// layout( location = 0 ) out vec4 oColor;

// void main()
// {
//     // Calculates the distance from the current particle to the distance
//     // Sprint's coordinates is between 0 and 1
//     float center = length(gl_PointCoord - vec2(0.5));
//     if (center > 0.5){
//         discard; //do nothing
//     }

//     oColor = vec4(oColor.rgb, oColor.a * (1.0 - center * 2.0));


// }