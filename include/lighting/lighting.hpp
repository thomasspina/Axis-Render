#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "pointLight.hpp"
#include "lightCaster.hpp"
#include "shaderProgram.hpp"

class Lighting {
private:
    glm::mat4 view;
    glm::mat4 projection;

    LightCaster lightCaster; // TODO: add multiple lightCasters
    std::vector<PointLight> pointLights;

    void updatePointLightsNormalMatrices();
public:
    Lighting();
    ~Lighting();

    void addLightCaster(LightCaster lightCaster);
    void addPointLight(PointLight pointLight);

    void updateView(glm::mat4 view);
    void updateProjection(glm::mat4 projection);

    void draw(ShaderProgram& shaderProgram);
};