#version 400

in  vec4 aPosition;
in  vec3 aColour;

out vec3 vColour;

void main(void) {
    gl_Position = aPosition;
    vColour = aColour;
}
