/*
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


const int ASCII_CHARS[10] = int[](32, 46, 58, 45, 61, 43, 42, 37, 64, 35);

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
    float brightness = dot(diffuse + specular, vec3(0.2126, 0.7152, 0.0722)); // Luminance formula

    // Convert brightness to ASCII character index
    int asciiIndex = int(brightness * 9.0);
    asciiIndex = clamp(asciiIndex, 0, 9); // Ensure it's within bounds

    float asciiBrightness = float(asciiIndex) / 9.0; // Scale to grayscale
    FragColour = vec4(vec3(asciiBrightness), 1.0); // Render ASCII as grayscale
}
*/
