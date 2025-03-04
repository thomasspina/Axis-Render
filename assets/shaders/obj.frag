#version 460 core
out vec4 FragColour;

in vec3 Diffuse;
in vec3 Specular;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
};

uniform Material material;


void main() 
{   
    vec3 diffuse = Diffuse * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = Specular * vec3(texture(material.specular, TexCoords));
    FragColour = vec4(diffuse + specular, 1.0);
}
