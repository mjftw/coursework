#version 150

precision highp float;

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;
in vec2 in_Texture;


// mvpmatrix is the result of multiplying the model, view, and projection matrices */
uniform mat4 mvpmatrix;

out vec3 ex_Color;
void main(void) {
// Multiply the mvp matrix by the vertex to obtain our final vertex position
    gl_Position = mvpmatrix * vec4(in_Position, 1.0);
//    float xxy = dot(in_Position, in_Texture);
//    float xxy = dot(in_Position, in_Position);
//    float xxx = max(0.5, 1.);
    float xxcxx = max(dot(in_Normal, vec3(1.0, 0., 0.)),0.9);
    ex_Color = in_Color * xxcxx;
}
