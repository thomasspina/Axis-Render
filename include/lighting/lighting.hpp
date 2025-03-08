#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "pointLight.hpp"
#include "lightCaster.hpp"
#include "shaderProgram.hpp"

class Lighting {
private:
    ShaderProgram pointLightShader;

    glm::mat4 view;
    glm::mat4 projection;

    LightCaster lightCaster; // TODO: add multiple lightCasters
    std::vector<PointLight> pointLights;
public:
    Lighting();
    ~Lighting();

    void addLightCaster(LightCaster lightCaster);
    void addPointLight(PointLight pointLight);

    void updateView(glm::mat4 view);
    void updateProjection(glm::mat4 projection);

    void drawPointLights();

    void setLightingUniforms(ShaderProgram& shaderProgram);
};