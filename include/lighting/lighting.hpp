#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "pointLight.hpp"
#include "lightCaster.hpp"
#include "shaderProgram.hpp"

// TODO: add max pointlight check

class Lighting {
private:
    glm::mat4 view;
    glm::mat4 projection;

    LightCaster lightCaster; // TODO: add multiple lightCasters
    std::vector<PointLight> pointLights;
public:
    Lighting();

    void updateView(const glm::mat4& view);
    void updateProjection(const glm::mat4& projection);

    void addLightCaster(LightCaster lightCaster);
    void addPointLight(PointLight pointLight);

    void drawPointLights(ShaderProgram& pointLightShader);

    void setLightingUniforms(ShaderProgram& lightingShader);
};