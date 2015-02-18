#version 400

precision highp float;

in  vec3 vEx_Color;

out vec4 fFragColor;

void main(void) {
    fFragColor = vec4(vEx_Color,1.0);
}
