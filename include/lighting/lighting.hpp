#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "camera.hpp"
#include "rendering/model.hpp"
#include "lighting/pointLight.hpp"
#include "lighting/lightCaster.hpp"
#include "shader/shaderProgram.hpp"

/**
 * @class Lighting
 * @brief A lighting system manager that handles directional and point lights
 * 
 * The Lighting class manages both a directional light (LightCaster) and multiple
 * point lights. It provides functionality to add and remove lights, set their
 * properties, and configure shaders with the appropriate lighting uniforms.
 * The system also handles transforming light positions to view space and drawing
 * visual representations of point lights.
 */
class Lighting {
private:
    glm::mat4 view;                   ///< Current view matrix for transforming lights to view space
    glm::mat4 projection;             ///< Current projection matrix for rendering point lights
    LightCaster lightCaster;          ///< Directional light (like the sun)
    std::vector<PointLight> pointLights; ///< Collection of point lights in the scene
    Camera* camera;                   ///< Pointer to the camera (needed for light placement in view frustum)
    Model* model;                     ///< Pointer to the model (needed for light placement relative to model)
    bool showPointLights = true;      ///< Whether to render the visual representation of point lights

public:
    /**
     * @brief Constructs a lighting system with optional camera and model references
     * 
     * @param camera Pointer to the scene camera (used for point light placement)
     * @param model Pointer to the scene model (used for determining light placement boundaries)
     */
    Lighting(Camera* camera = nullptr, Model* model = nullptr) : camera(camera), model(model) {}

    /**
     * @brief Gets whether point light visual representations are shown
     * @return true if point lights are visible, false otherwise
     */
    bool getShowPointLights() const { return showPointLights; }

    /**
     * @brief Gets the number of point lights in the scene
     * @return Number of active point lights
     */
    int getNPointLights() const { return pointLights.size(); }

    /**
     * @brief Gets a pointer to the directional light
     * @return Pointer to the LightCaster object
     */
    LightCaster* getLightCasterPointer() { return &lightCaster; }

    /**
     * @brief Gets a pointer to the collection of point lights
     * @return Pointer to the vector of PointLight objects
     */
    std::vector<PointLight>* getPointLightsPointer() { return &pointLights; }

    /**
     * @brief Sets the camera reference
     * @param camera Pointer to the scene camera
     */
    void setCamera(Camera* camera) { this->camera = camera; }

    /**
     * @brief Sets the model reference
     * @param model Pointer to the scene model
     */
    void setModel(Model* model) { this->model = model; }

    /**
     * @brief Sets the current view matrix
     * @param view The view transformation matrix
     */
    void setView(const glm::mat4& view) { this->view = view; }

    /**
     * @brief Sets the current projection matrix
     * @param projection The projection transformation matrix
     */
    void setProjection(const glm::mat4& projection) { this->projection = projection; }

    /**
     * @brief Sets the directional light properties
     * @param lightCaster A LightCaster object to copy
     */
    void setLightCaster(LightCaster lightCaster) { this->lightCaster = lightCaster; }

    /**
     * @brief Toggles visibility of point light visual representations
     * @param showPointLights Whether to show point lights
     */
    void setShowPointLights(bool showPointLights) { this->showPointLights = showPointLights; }

    /**
     * @brief Adds a new point light in the camera's view frustum
     * 
     * Creates a new point light at a random position within the camera's view frustum,
     * at a random depth between the near plane and 3x the model radius. If the maximum
     * number of point lights is reached (MAX_POINT_LIGHTS), the oldest light is removed.
     */
    void addPointLight();

    /**
     * @brief Removes the most recently added point light
     */
    void removePointLight() { pointLights.pop_back(); }

    /**
     * @brief Renders the visual representation of all point lights
     * 
     * Uses the provided shader to draw cube representations of each point light
     * with the appropriate color. Does nothing if showPointLights is false.
     * 
     * @param pointLightShader Shader program for rendering point light objects
     */
    void drawPointLights(ShaderProgram& pointLightShader);

    /**
     * @brief Configures shader uniforms for all lights in the scene
     * 
     * Sets uniforms for the directional light and all point lights, including
     * transforming point light positions to view space. Also sets the number
     * of active point lights.
     * 
     * @param lightingShader Shader program to configure with lighting uniforms
     */
    void setUniformsForShaderProgram(ShaderProgram& lightingShader);
};