#version 330 core

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
out vec4 FragColor;

uniform Light light;
uniform vec3 viewPos;
// Este nombre coincide con el que envia Mesh.h.
uniform sampler2D texture_diffuse1;
uniform bool sinIluminacion;
uniform bool usarTextura;
uniform vec3 colorObjeto;

void main()
{
    vec4 base = vec4(1.0);
    if (usarTextura)
        base = texture(texture_diffuse1, TexCoords);
    if (base.a < 0.1)
        discard;

    // Sol, luna y cielo: no reciben la luz del escenario.
    if (sinIluminacion)
    {
        FragColor = vec4(base.rgb * colorObjeto, base.a);
        return;
    }

    // El mismo esquema ambiental, difuso y especular de tu practica.
    vec3 normal = normalize(Normal);
    vec3 direccionLuz = normalize(light.position - FragPos);
    float difuso = max(dot(normal, direccionLuz), 0.0);

    vec3 direccionCamara = normalize(viewPos - FragPos);
    vec3 reflejo = reflect(-direccionLuz, normal);
    float especular = 0.0;
    if (difuso > 0.0)
        especular = pow(max(dot(direccionCamara, reflejo), 0.0), 16.0);

    vec3 resultado = (light.ambient + light.diffuse * difuso) * base.rgb;
    resultado += light.specular * light.diffuse * especular;
    FragColor = vec4(resultado, base.a);
}