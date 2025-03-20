#pragma once

#include <glm/glm.hpp>

#include "shader/shaderProgram.hpp"
#include "utils/constants.hpp"


/**
 * @class LightCaster
 * @brief A directional light source for 3D rendering
 * 
 * The LightCaster class simulates a directional light source like the sun,
 * where all light rays are parallel. It can be defined using either angular
 * coordinates (azimuth/elevation) or a direction vector. The light uses
 * default values from constants.hpp for intensity and color.
 */
class LightCaster {
private:
    glm::vec3 direction;  ///< Normalized direction vector (points from light toward scene)
    float intensity;      ///< Light brightness multiplier (set to DEFAULT_LIGHT_INTENSITY by default)
    glm::vec3 colour;     ///< Light color (RGB) (set to DEFAULT_LIGHT_COLOUR by default)
    float azimuth;        ///< Horizontal angle in degrees (0-360°)
    float elevation;      ///< Vertical angle in degrees (-90° to 90°)

    /**
     * @brief Converts angular coordinates to a normalized direction vector
     * @param azimuth Horizontal angle in degrees
     * @param elevation Vertical angle in degrees
     * @return Normalized direction vector
     */
    glm::vec3 anglestoDirection(float azimuth, float elevation);

    /**
     * @brief Converts a direction vector to angular coordinates
     * @param direction Normalized direction vector
     * @return Pair of angles (azimuth, elevation) in degrees
     */
    std::pair<float, float> directionToAngles(const glm::vec3& direction);

public:
    /**
     * @brief Default constructor
     * 
     * Creates a light with default intensity (DEFAULT_LIGHT_INTENSITY) and
     * default color (DEFAULT_LIGHT_COLOUR). The light direction is set to
     * DEFAULT_LIGHT_CASTER_DIRECTION.
     */
    LightCaster() : LightCaster(DEFAULT_LIGHT_CASTER_DIRECTION) {}

    /**
     * @brief Constructs a directional light using angular coordinates
     * 
     * Creates a light with default intensity (DEFAULT_LIGHT_INTENSITY) and
     * default color (DEFAULT_LIGHT_COLOUR).
     * 
     * @param azimuth Horizontal angle in degrees (0-360° where 0° is along negative z-axis)
     * @param elevation Vertical angle in degrees (-90° to 90° where 90° points upward)
     */
    LightCaster(float azimuth, float elevation);

    /**
     * @brief Constructs a directional light using a direction vector
     * 
     * Creates a light with default intensity (DEFAULT_LIGHT_INTENSITY) and
     * default color (DEFAULT_LIGHT_COLOUR). Automatically calculates the
     * corresponding azimuth and elevation angles.
     * 
     * @param direction Vector pointing from the light source toward the scene (will be normalized)
     */
    LightCaster(const glm::vec3& direction);

    /**
     * @brief Gets the light's normalized direction vector
     * @return Direction vector pointing from light toward scene
     */
    glm::vec3 getDirection() const { return direction; }

    /**
     * @brief Gets the light's intensity value
     * @return Light intensity multiplier
     */
    float getIntensity() const { return intensity; }

    /**
     * @brief Gets the light's color
     * @return RGB color vector
     */
    glm::vec3 getColour() const { return colour; }

    /**
     * @brief Gets the light's azimuth angle
     * @return Azimuth in degrees (0-360°)
     */
    float getAzimuth() const { return azimuth; }

    /**
     * @brief Gets the light's elevation angle
     * @return Elevation in degrees (-90° to 90°)
     */
    float getElevation() const { return elevation; }

    /**
     * @brief Sets the light's color
     * @param newColour RGB color vector
     */
    void setColour(const glm::vec3& newColour) { colour = newColour; }

    /**
     * @brief Sets the light's intensity
     * @param newIntensity Brightness multiplier
     */
    void setIntensity(float newIntensity) { intensity = newIntensity; }

    /**
     * @brief Sets the light's direction using a vector
     * 
     * Updates both the direction vector and the corresponding azimuth/elevation angles.
     * 
     * @param newDirection Direction vector (will be normalized)
     */
    void setDirection(const glm::vec3& newDirection);

    /**
     * @brief Updates the light direction using angular coordinates
     * 
     * Updates both the azimuth/elevation angles and the corresponding direction vector.
     * 
     * @param azimuth Horizontal angle in degrees
     * @param elevation Vertical angle in degrees
     */
    void setDirection(float azimuth, float elevation);

    /**
     * @brief Configures shader uniforms with this light's properties
     * 
     * Sets directional light uniforms including direction, ambient, diffuse, and specular
     * components based on the light's color and intensity values. Uses constants from
     * constants.hpp (DEFAULT_LIGHT_AMBIENT, DEFAULT_LIGHT_DIFFUSE, DEFAULT_LIGHT_SPECULAR).
     * 
     * @param shaderProgram Reference to the shader program to configure
     */
    void setUniformsForShaderProgram(ShaderProgram& shaderProgram);
};