#version 400
precision highp float;

in  vec3 vColour;
out vec4 FragColor;

void main(void) {
    FragColor = vec4(vColour,1.0);
}
