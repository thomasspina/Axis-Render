#version 460 core
// Must be the very first line, no spaces or blank lines before

// Output final fragment color
out vec4 FragColour;

// Inputs from the vertex shader
in vec3 Diffuse;    // Diffuse lighting factor
in vec3 Specular;   // Specular lighting factor
in vec2 TexCoords;  // Texture coordinates

// Material structure containing texture samplers and counts
struct Material {
    sampler2D texture_diffuse[8];
    sampler2D texture_specular[8];
    int diffuse_count;
    int specular_count;
};

uniform Material material; // Uniform for the material

// -------------------------------------------------------------------
// The 'character' function simulates a 5x5 ASCII character pattern.
// It scales the local coordinate p into a 5x5 grid and then uses
// the integer 'n' as a bit mask to decide if the pixel is "on" or "off".
// -------------------------------------------------------------------
float character(int n, vec2 p) {
    // Scale and translate p to a 5x5 grid
    p = floor(p * vec2(-4.0, 4.0) + 2.5);
    
    // Only consider positions within the grid (0 to 4)
    if (clamp(p.x, 0.0, 4.0) == p.x && clamp(p.y, 0.0, 4.0) == p.y) {
        int a = int(round(p.x) + 5.0 * round(p.y)); // Convert 2D pos to 1D index (0-24)
        // Check if bit 'a' in 'n' is set (returns 1.0 if on, 0.0 if off)
        if (((n >> a) & 1) == 1)
            return 1.0;
    }
    return 0.0;
}

void main()
{
    // Compute combined lighting from the diffuse and specular textures.
    vec3 diffuseColor = vec3(0.0);
    vec3 specularColor = vec3(0.0);
    
    // Blend diffuse textures using the provided diffuse count
    for (int i = 0; i < material.diffuse_count; i++) {
        diffuseColor += texture(material.texture_diffuse[i], TexCoords).rgb;
    }
    diffuseColor *= Diffuse;
    
    // Blend specular textures; note we only use the red channel since specular maps are grayscale.
    for (int i = 0; i < material.specular_count; i++) {
        specularColor += texture(material.texture_specular[i], TexCoords).r * vec3(1.0);
    }
    specularColor *= Specular;
    
    // Convert combined color to grayscale using a standard luminance formula.
    float brightness = dot(diffuseColor + specularColor, vec3(0.2126, 0.7152, 0.0722));
    
    // Choose a bitmask pattern (an integer) based on brightness.
    // These numbers are arbitrary examples encoding 5x5 bit patterns.
    int n = 4096;
    if (brightness > 0.2) n = 65600;
    if (brightness > 0.3) n = 163153;
    if (brightness > 0.4) n = 15255086;
    if (brightness > 0.5) n = 13121101;
    if (brightness > 0.6) n = 15252014;
    if (brightness > 0.7) n = 13195790;
    if (brightness > 0.8) n = 11512810;
    
    // Divide the screen into cells; here each cell is assumed to be 8x8 pixels.
    // gl_FragCoord.xy gives the pixel's coordinate on the screen.
    vec2 p = mod(gl_FragCoord.xy / 8.0, 2.0) - vec2(1.0);
    
    // Get the mask value (1.0 for "on" pixel, 0.0 for "off")
    float mask = character(n, p);
    
    // Output the final fragment color as grayscale based on the mask.
    FragColour = vec4(vec3(mask), 1.0);
}
