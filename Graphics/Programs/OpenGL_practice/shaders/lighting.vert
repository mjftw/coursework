#version 330 core
in vec3 vertexPos_modelspace;
in vec2 vertexUV;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 lightPos_modelspace;
uniform vec3 camPos_modelspace;
out vec3 normal;
out vec3 lightPos;
out vec3 camPos;
out vec2 UV;

void main(void)
{
    gl_Position = P * V * M * vec4(vertexPos_modelspace, 1);

    normal = normalize(vertexPos_modelspace);
    lightPos = (vec4(lightPos_modelspace, 1) * M).xyz;
    camPos = (vec4(camPos_modelspace, 1) * V * M).xyz;
    UV = vertexUV;
}
