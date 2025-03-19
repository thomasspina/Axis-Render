#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "camera.hpp"
#include "rendering/model.hpp"
#include "lighting/pointLight.hpp"
#include "lighting/lightCaster.hpp"
#include "shader/shaderProgram.hpp"

class Lighting {
private:
    glm::mat4 view;
    glm::mat4 projection;

    LightCaster lightCaster;
    std::vector<PointLight> pointLights;
    int nPointLights = 0;

    glm::vec3 anglestoDirection(float azimuth, float elevation);
    float azimuth;
    float elevation;

    Camera* camera; // Camera is needed for random point light generation in view frustum
    Model* model; // Model is needed for the model radius and center

    bool isDrawPointLights = true;
public:
    Lighting() = default;
    void setCamera(Camera* camera) { this->camera = camera; }
    void setModel(Model* model) { this->model = model; }

    void updateView(const glm::mat4& view);
    void updateProjection(const glm::mat4& projection);

    void addLightCaster(LightCaster lightCaster);
    float* getCasterAzimuth() { return &azimuth; } // getter for the UI azimuth slider
    float* getCasterElevation() { return &elevation; } // getter for the UI elevation slider
    void updateCasterDirection(); // updates the light caster direction based on the azimuth and elevation
    bool* drawPointLightsBool() { return &isDrawPointLights; } // getter for the UI point light toggle

    LightCaster* getLightCaster() { return &lightCaster; } // light caster getter for the UI intensity sliders and colour pickers
    std::vector<PointLight>& getPointLights() { return pointLights; }
    int* getNPointLights() { return &nPointLights; } // getter for the UI point light count input

    /// @brief Add a randomly generated point light to the scene, removes first one if the limit is reached
    void addPointLight();

    void removePointLight() { pointLights.pop_back(); } // removes the last point light from the scene

    void drawPointLights(ShaderProgram& pointLightShader);

    void setLightingUniforms(ShaderProgram& lightingShader);
};