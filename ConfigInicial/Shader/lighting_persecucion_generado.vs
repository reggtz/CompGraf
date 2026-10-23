#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform float animCabeza;
vec3 animDoblar(vec3 p) {
float peso = smoothstep(-0.17680998, 0.06161803, p.y) * smoothstep(-0.00503001, 0.24920997, p.z);
vec3 pivote = vec3(0.0, -0.03110400, 0.01086000);
float a = animCabeza * peso;
vec3 q = p - pivote;
return pivote + vec3(q.x, cos(a)*q.y - sin(a)*q.z, sin(a)*q.y + cos(a)*q.z);
}

vec3 animDoblarNormal(vec3 p, vec3 n) {
    float e = 0.0005;
    vec3 dx = (animDoblar(p + vec3(e,0,0)) - animDoblar(p - vec3(e,0,0))) / (2.0*e);
    vec3 dy = (animDoblar(p + vec3(0,e,0)) - animDoblar(p - vec3(0,e,0))) / (2.0*e);
    vec3 dz = (animDoblar(p + vec3(0,0,e)) - animDoblar(p - vec3(0,0,e))) / (2.0*e);
    return normalize(cross(dy,dz)*n.x + cross(dz,dx)*n.y + cross(dx,dy)*n.z);
}
void main()
{
vec3 animPos = animDoblar(position);
vec3 animNormal = animDoblarNormal(position, normal);

    gl_Position = projection * view *  model * vec4(animPos, 1.0f);
    FragPos = vec3(model * vec4(animPos, 1.0f));
    Normal = mat3(transpose(inverse(model))) * animNormal;
    TexCoords = texCoords;
}