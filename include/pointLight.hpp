#pragma once

#include <glm/glm.hpp>

#include "mesh.hpp"
#include "shaderProgram.hpp"

class PointLight {
private:
    ShaderProgram shaderProgram; // Different shader program since light isn't affected by light
    Mesh mesh;

    glm::vec3 position;
    glm::vec3 colour;

    float intensity;

    float constant;
    float linear;
    float quadratic;
public:
    PointLight(glm::vec3 position, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f), float intensity = 1.0f, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);
    ~PointLight();

    void drawLight(); // Draw the light source

    void setUniforms(const ShaderProgram& shaderProgram); // Set the uniforms coming from the light source
};