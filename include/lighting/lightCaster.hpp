#pragma once

#include <glm/glm.hpp>

#include "shaderProgram.hpp"

class LightCaster {
private:
    glm::vec3 direction;
    float intensity;
    glm::vec3 colour;
public:
    LightCaster() = default;
    LightCaster(const glm::vec3& direction, float intensity);

    void setUniforms(ShaderProgram& shaderProgram);
};