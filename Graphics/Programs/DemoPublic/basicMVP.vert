#version 400

in  vec4 aPosition;
in  vec3 aColour;
uniform mat4 uMVP;
out vec3 vColour;

void main(void) {
    gl_Position = uMVP * aPosition;
    vColour = aColour;
}
