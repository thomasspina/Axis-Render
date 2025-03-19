#pragma once

#include <glm/glm.hpp>

#include "shader/shaderProgram.hpp"


class LightCaster {
private:
    glm::vec3 direction;
    float intensity;
    glm::vec3 colour;
public:
    LightCaster() = default;
    LightCaster(const glm::vec3& direction, float intensity);

    glm::vec3 getDirection() const { return direction; }
    float getIntensity() const { return intensity; }
    float* getIntensity() { return &intensity; } // intensity getter for the UI intensity slider
    glm::vec3 getColour() const { return colour; }
    glm::vec3* getColour() { return &colour; } // colour getter for the UI colour picker

    void setDirection(const glm::vec3& direction);

    void setUniforms(ShaderProgram& shaderProgram);
};