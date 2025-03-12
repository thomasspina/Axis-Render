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
public:
    Lighting() = default;

    void updateView(const glm::mat4& view);
    void updateProjection(const glm::mat4& projection);

    void addLightCaster(LightCaster lightCaster);

    /// @brief Add a point light to the scene, removes first one if the limit is reached
    void addPointLight(PointLight pointLight);

    void drawPointLights(ShaderProgram& pointLightShader);

    void setLightingUniforms(ShaderProgram& lightingShader);
};