#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Diffuse;
out vec3 Specular;
out vec2 TexCoords;

struct PointLight {
    vec3 position;

    // attenuation factors
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_POINT_LIGHTS 10
uniform int nr_point_lights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform DirLight dirLight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

// ======== FUNCTION DECLARATIONS ========
mat3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


// ======== MAIN ========
void main() 
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;

    vec3 Normal = normalize(normalMatrix * aNormal); // normal in view space (normal matrix is the inverse transpose of the view*model matrix)
    vec3 Position = vec3(view * model * vec4(aPos, 1.0)); // position in view space
    vec3 ViewDir = normalize(-Position);

    // directional light
    mat3 dirLightMatrix = CalcDirLight(dirLight, Normal, ViewDir);

    // point lights
    mat3 pointLightMatrix = mat3(0.0);
    for (int i = 0; i < nr_point_lights; i++)
    {
        pointLightMatrix += CalcPointLight(pointLights[i], Normal, Position, ViewDir);
    }


    Diffuse = dirLightMatrix[0] + dirLightMatrix[1] + pointLightMatrix[0] + pointLightMatrix[1];
    Specular = dirLightMatrix[2] + pointLightMatrix[2];
}


mat3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // TODO: find a solution for the hard-coded shininess value
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    return mat3(
            ambient,
            diffuse,
            specular
        );
}

mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // TODO: find a solution for the hard-coded shininess value
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return mat3(
            ambient,
            diffuse,
            specular
        );
}
