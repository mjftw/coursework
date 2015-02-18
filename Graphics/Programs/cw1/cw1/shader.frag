#version 150
precision highp float;

flat in  vec3 ex_Color;
out vec4 FragColor;

void main(void) {
    FragColor = vec4(ex_Color,1.0);
}
