#version 400

precision highp float;

in vec4 aPosition;
in vec3 aColor;

// mvpmatrix is the result of multiplying the model, view, and projection matrices */
uniform mat4 uMVPmatrix;

out vec3 vEx_Color;
void main(void) {
// Multiply the mvp matrix by the vertex to obtain our final vertex position
    gl_Position = uMVPmatrix * aPosition;
//    gl_Position = uMVPmatrix * vec4(aPosition, 1.0);
//    gl_Position = vec4(aPosition, 1.0);

    vEx_Color = aColor;
}
