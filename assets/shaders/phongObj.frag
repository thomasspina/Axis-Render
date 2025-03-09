#version 460 core
out vec4 FragColour;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

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

struct Material {
    sampler2D texture_diffuse[8];
    sampler2D texture_specular[8];
    int diffuse_count;
    int specular_count;
};

uniform Material material;

// ======== FUNCTION DECLARATIONS ========
mat3 CalcDirLight(DirLight light, vec3 normal);
mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos);

void main()
{

    // directional light
    mat3 dirLightCalc = CalcDirLight(dirLight, Normal);

    // point lights
    mat3 pointLightCalc = mat3(0.0);
    for (int i = 0; i < nr_point_lights; i++) {
        pointLightCalc += CalcPointLight(pointLights[i], Normal, FragPos);
    }

    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    
    // blend diffuse textures
    for (int i = 0; i < material.diffuse_count; i++) {
        diffuse += vec3(texture(material.texture_diffuse[i], TexCoords));
    }
    diffuse *= Diffuse; // apply diffuse lighting
    
    // blend specular textures
    for (int i = 0; i < material.specular_count; i++) {
        specular += vec3(texture(material.texture_specular[i], TexCoords).r); // .r is necessary because specular maps are grayscale and only red is set
    }
    specular *= Specular; // apply specular lighting
    
    FragColour = vec4(diffuse + specular, 1.0);
}

mat3 CalcDirLight(DirLight light, vec3 normal)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, reflectDir), 0.0), 32.0); // TODO: find a solution for the hard-coded shininess value
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

mat3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
    vec3 lightPos = vec3(view * vec4(light.position, 1.0)); // light position in view space

    vec3 lightDir = normalize(lightPos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, reflectDir), 0.0), 32.0); // TODO: find a solution for the hard-coded shininess value
    // attenuation
    float distance = length(lightPos - fragPos);
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