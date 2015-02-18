#version 400

precision highp float;

in vec4 aPosition;
//in vec3 aColor;
in vec2 aTexture;

// mvpmatrix is the result of multiplying the model, view, and projection matrices */
uniform mat4 uMVPmatrix;

//out vec3 vEx_Color;
out vec2 vTexture;

void main(void) {
// Multiply the mvp matrix by the vertex to obtain our final vertex position
    gl_Position = uMVPmatrix * aPosition;
    //    vEx_Color = aColor;
//    gl_Position = vec4(aTexture, 0., 1.);
    vTexture = aTexture;
}
