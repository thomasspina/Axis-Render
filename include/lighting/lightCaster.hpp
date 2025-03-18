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
    glm::vec3 getColour() const { return colour; }

    void setDirection(const glm::vec3& direction);
    void setIntensity(float intensity);
    void setColour(const glm::vec3& colour);

    void setUniforms(ShaderProgram& shaderProgram);
};