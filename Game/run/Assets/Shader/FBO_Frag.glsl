#version 300 es
precision highp float;

in vec2 UV;

out vec4 fragColor;

uniform sampler2D fboTexture;
uniform sampler2D lightMap;

void main() {
    vec3 colour = vec3(texture(fboTexture, UV));

    // Colour inverse
    //fragColor = vec4(vec3(1.0f - colour), 1.0f);
    //fragColor = vec4(colour.x, 0, 0, 1.0);
    fragColor = vec4(colour, 1.0);
}
