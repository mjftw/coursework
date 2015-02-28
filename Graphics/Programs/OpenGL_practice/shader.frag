#version 330 core
flat in vec3 fragmentColour;
in vec3 normal;
in vec3 lightPos;
in vec3 camPos;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform int Sn;
uniform vec3 lightCol;

out vec3 colour;

vec3 rotate(vec3 inVec, float angle, vec3 axis)
{
    vec3 u = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    mat4 rotMat = mat4(c+(u.x*u.x*oc),       (u.x*u.y*oc)-(u.z*s), (u.x*u.z*oc)+(u.y*s), 0.0,
                      (u.y*u.x*oc)+(u.z*s),  c+(u.y*u.y*oc),       (u.y*u.z*oc)-(u.x*s), 0.0,
                      (u.z*u.x*oc)-(u.y*s),  (u.z*u.y*oc)+(u.x*s), c+(u.z*u.z*oc),       0.0,
                      0.0,                   0.0,                  0.0,                  1.0);
    return (rotMat * vec4(inVec, 1 )).xyz;
}

float ambient(float ka)
{
    float light = ka;
    return light;
}

float diffuse(float kd)
{
    float light = kd * (dot(lightPos, normal) / (length(lightPos) * length(normal))); //Cos(theta) theta = angle between light source & normal
    return light;
}

float specular(float ks)
{

    vec3 reflectDir = rotate(lightPos, 180, normal);
    float cosPhi = dot(reflectDir, camPos) / (length(reflectDir) * length(camPos));
    float light = ks;
    for(int i=0; i<Sn; i++)
    {
        light = light * cosPhi;
    }

    return light;
}



void main(void)
{
    vec3 temp;
    temp.x = fragmentColour.x * lightCol.x * (ambient(Ka.x) + diffuse(Kd.x) + specular(Ks.x)); //red
    temp.y = fragmentColour.y * lightCol.y * (ambient(Ka.y) + diffuse(Kd.y) + specular(Ks.y)); //green
    temp.z = fragmentColour.z * lightCol.z * (ambient(Ka.z) + diffuse(Kd.z) + specular(Ks.z)); //blue
    colour =  temp;
}

