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

void main()
{
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    
    // Blend diffuse textures
    for (int i = 0; i < material.diffuse_count; i++) {
        diffuse += vec3(texture(material.texture_diffuse[i], TexCoords));
    }
    diffuse *= Diffuse; // Apply diffuse lighting
    
    // Blend specular textures
    for (int i = 0; i < material.specular_count; i++) {
        specular += vec3(texture(material.texture_specular[i], TexCoords).r);
    }
    specular *= Specular; // Apply specular lighting

    // Combine lighting and convert to grayscale
    vec3 combined = diffuse + specular;
    float brightness = dot(combined, vec3(0.2126, 0.7152, 0.0722)); // Luminance formula

    // Output grayscale color
    FragColour = vec4(vec3(brightness), 1.0); // Apply grayscale to final color
}
