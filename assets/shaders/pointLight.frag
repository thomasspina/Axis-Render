#version 460 core

out vec4 FragColor;

uniform vec3 lightColour;

void main()
{
    FragColor = vec4(lightColour, 1.0); // TODO: set the fragment's colour based on the light's colour
}