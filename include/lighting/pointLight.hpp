#pragma once

#include <glm/glm.hpp>

#include "mesh.hpp"
#include "object.hpp"
#include "shaderProgram.hpp"

class PointLight : public Mesh, public Object {
private:
    ShaderProgram shaderProgram; // Different shader program since light isn't affected by light

    glm::vec3 position;
    glm::vec3 colour;

    float intensity;

    float constant;
    float linear;
    float quadratic;
public:
    PointLight(glm::vec3 position, float size, float intensity);
    ~PointLight();

    void draw(ShaderProgram& shader) override; // Draw the light source

    void setUniforms(ShaderProgram& shaderProgram, const std::string& i); // Set the uniforms coming from the light source
};