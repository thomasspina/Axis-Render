#version 460 core
out vec4 FragColour;

in vec3 Diffuse;
in vec3 Specular;
in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse[8];
    sampler2D texture_specular[8];
    int diffuse_count;
    int specular_count;
};

uniform Material material;


// TODO: future improvement: add different texture blending modes
void main()
{
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

