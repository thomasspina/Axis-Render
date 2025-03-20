#pragma once

#include <glm/glm.hpp>
#include <string>

#include "object.hpp"
#include "shader/shaderProgram.hpp"

/**
 * @class PointLight
 * @brief A point light source for 3D rendering with attenuation
 * 
 * The PointLight class simulates a light that radiates in all directions from a point
 * in 3D space. It inherits from Object to have position and transformation capabilities.
 * The light includes a visible mesh representation (cube) and realistic attenuation
 * using constant, linear, and quadratic factors.
 */
class PointLight : public Object {
private:
    GLuint VAO, VBO, EBO;             ///< OpenGL buffer objects for the light's visual representation
    int nIndices;                     ///< Number of indices in the mesh's element buffer
    glm::vec3 colour;                 ///< Light color (RGB)
    float intensity;                  ///< Light brightness multiplier
    float constant;                   ///< Constant attenuation factor (DEFAULT_LIGHT_CONSTANT)
    float linear;                     ///< Linear attenuation factor (DEFAULT_LIGHT_LINEAR)
    float quadratic;                  ///< Quadratic attenuation factor (DEFAULT_LIGHT_QUADRATIC)

    /**
     * @brief Sets up the cube mesh used to visually represent the light
     * 
     * Creates and configures the VAO, VBO, and EBO for rendering a cube
     * with the specified size.
     * 
     * @param size The side length of the cube
     */
    void setupLightMesh(float size);

public:
    /**
     * @brief Constructs a point light at the given position with a visual representation
     * 
     * Creates a point light with default intensity (DEFAULT_LIGHT_INTENSITY) and
     * default color (DEFAULT_LIGHT_COLOUR). Default attenuation values are also
     * applied from constants.hpp.
     * 
     * @param position The 3D position of the light in world space
     * @param size The size of the cube used to visually represent the light
     */
    PointLight(glm::vec3 position, float size);

    /**
     * @brief Gets the light's color
     * @return RGB color vector
     */
    glm::vec3 getColour() const { return colour; }

    /**
     * @brief Gets the light's intensity value
     * @return Light intensity multiplier
     */
    float getIntensity() const { return intensity; }

    /**
     * @brief Sets the light's color
     * @param colour RGB color vector
     */
    void setColour(const glm::vec3& colour) { this->colour = colour; }

    /**
     * @brief Sets the light's intensity
     * @param intensity Brightness multiplier
     */
    void setIntensity(float intensity) { this->intensity = intensity; }

    /**
     * @brief Renders the light's visual representation (cube)
     * 
     * Binds the VAO and draws the light's cube mesh. This method only renders
     * the visual representation of the light - it does not affect lighting calculations.
     */
    void draw();

    /**
     * @brief Configures shader uniforms with this light's properties
     * 
     * Sets point light uniforms including position, color components, and attenuation
     * factors. Uses constants from constants.hpp for default multipliers.
     * 
     * @param shaderProgram Reference to the shader program to configure
     * @param i String index for the light in a lights array (e.g., "0" for first light)
     */
    void setUniformsForShaderProgram(ShaderProgram& shaderProgram, const std::string& i);
};