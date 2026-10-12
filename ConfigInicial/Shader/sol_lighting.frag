#version 330 core

#define NUMBER_OF_POINT_LIGHTS 4

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform int transparency;
uniform float opacity;
uniform bool esCristal;
uniform sampler2D texture_diffuse1;
uniform bool sinIluminacion;

// Function prototypes
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

void main( )
{
    // El plano del cielo solo muestra su textura.
    if (sinIluminacion)
    {
        color = vec4(texture(texture_diffuse1, TexCoords).rgb, 1.0);
        return;
    }

    // Properties
    vec3 norm = normalize( Normal );
    vec3 viewDir = normalize( viewPos - FragPos );
    
    // Directional lighting
    vec3 result = CalcDirLight( dirLight, norm, viewDir );
    
    // Point lights
    for ( int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++ )
    {
        result += CalcPointLight( pointLights[i], norm, FragPos, viewDir );
    }
    
    // Spot light
    result += CalcSpotLight( spotLight, norm, FragPos, viewDir );
 	
    //color = vec4( result,texture(texture_diffuse1, TexCoords).rgb );
	//  if(color.a < 0.1 && transparency==1)
    //    discard;

    float alphaTextura = esCristal
        ? 1.0
        : texture(texture_diffuse1, TexCoords).a;

    if (transparency == 1 && alphaTextura < 0.1)
        discard;

    color = vec4(result, alphaTextura * opacity);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize( -light.direction );
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
    
    // Combine results
    //vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );
    //vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );
    //vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );
    
    // Para el cristal usamos un tono claro y reflejos blancos.
    vec3 base = esCristal
        ? vec3(0.85, 0.95, 1.0)
        : texture(texture_diffuse1, TexCoords).rgb;

    vec3 brillo = esCristal
        ? vec3(1.0)
        : vec3(1.0);

    vec3 ambient = light.ambient * base;
    vec3 diffuse = light.diffuse * diff * base;
    vec3 specular = light.specular * spec * brillo;

    return ( ambient + diffuse + specular );
}

// Calculates the color when using a point light.
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
    
    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );
    
    // Combine results
    //vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );
    //vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );
    //vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );
    
    // Para el cristal usamos un tono claro y reflejos blancos.
    vec3 base = esCristal
        ? vec3(0.85, 0.95, 1.0)
        : texture(texture_diffuse1, TexCoords).rgb;

    vec3 brillo = esCristal
        ? vec3(1.0)
        : vec3(1.0);

    vec3 ambient = light.ambient * base;
    vec3 diffuse = light.diffuse * diff * base;
    vec3 specular = light.specular * spec * brillo;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ( ambient + diffuse + specular );
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
    
    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );
    
    // Spotlight intensity
    float theta = dot( lightDir, normalize( -light.direction ) );
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 );
    
    // Combine results
    //vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );
    //vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );
    //vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );
    
    // Para el cristal usamos un tono claro y reflejos blancos.
    vec3 base = esCristal
        ? vec3(0.85, 0.95, 1.0)
        : texture(texture_diffuse1, TexCoords).rgb;

    vec3 brillo = esCristal
        ? vec3(1.0)
        : vec3(1.0);

vec3 ambient = light.ambient * base;
vec3 diffuse = light.diffuse * diff * base;
vec3 specular = light.specular * spec * brillo;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return ( ambient + diffuse + specular );
}