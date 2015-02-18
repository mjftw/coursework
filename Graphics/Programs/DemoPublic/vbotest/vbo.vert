#version 400

precision highp float;

in vec4 aPosition;
in vec3 aColor;

uniform mat4 uMVPmatrix;

out vec3 vEx_Color;

void main(void) {
    gl_Position = uMVPmatrix * aPosition;
    vEx_Color = aColor;
}
