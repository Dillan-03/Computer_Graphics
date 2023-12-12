#version 430 core

in vec3 v2fColor;
in vec2 v2fTexCoord;

out vec4 fragColor;

uniform sampler2D uTexture; // Texture sampler

void main() {
    // Sample the texture
    vec4 texColor = texture(uTexture, v2fTexCoord);
    
    // Combine texture color with vertex color (modulate)
    fragColor = texColor * vec4(v2fColor, 1.0);
}