#version 400
precision highp float;
//in  vec3 vEx_Color;
in vec2 vTexture;


out vec4 fFragColor;

uniform sampler2D uTex;

void main(void) {
    fFragColor = texture(uTex, vTexture) * vec4(1.0, 1.0, 1.0, 1.0);
//    fFragColor = vec4(vTexture[0], vTexture[1], 0., 1.);
//    fFragColor = vec4(vTexture, vTexture);
}
