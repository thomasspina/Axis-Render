#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "lighting/pointLight.hpp"
#include "lighting/lightCaster.hpp"
#include "shader/shaderProgram.hpp"

class Lighting {
private:
    glm::mat4 view;
    glm::mat4 projection;

    LightCaster lightCaster;
    std::vector<PointLight> pointLights;

    glm::vec3 anglestoDirection(float azimuth, float elevation);
    float azimuth;
    float elevation;

public:
    Lighting() = default;

    void updateView(const glm::mat4& view);
    void updateProjection(const glm::mat4& projection);

    void addLightCaster(LightCaster lightCaster);
    float* getCasterAzimuth() { return &azimuth; } // getter for the UI azimuth slider
    float* getCasterElevation() { return &elevation; } // getter for the UI elevation slider
    void updateCasterDirection(); // updates the light caster direction based on the azimuth and elevation

    LightCaster* getLightCaster() { return &lightCaster; } // light caster getter for the UI intensity sliders and colour pickers

    /// @brief Add a point light to the scene, removes first one if the limit is reached
    void addPointLight(PointLight pointLight);

    void drawPointLights(ShaderProgram& pointLightShader);

    void setLightingUniforms(ShaderProgram& lightingShader);
};