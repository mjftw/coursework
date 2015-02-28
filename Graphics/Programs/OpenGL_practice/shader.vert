#version 330 core
in vec3 vertexPos_modelspace;
in vec3 vertexColour;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 lightPos_modelspace;
uniform vec3 camPos_modelspace;
flat out vec3 fragmentColour;
out vec3 normal;
out vec3 lightPos;
out vec3 camPos;

void main(void)
{
    gl_Position = P * V * M * vec4(vertexPos_modelspace, 1);
    fragmentColour = vertexColour;
    normal = normalize(vertexPos_modelspace);
    lightPos = (vec4(lightPos_modelspace, 1) * M).xyz;
    camPos = (vec4(camPos_modelspace, 1) * M).xyz;
}
