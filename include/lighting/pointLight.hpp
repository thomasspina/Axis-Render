#pragma once

#include <glm/glm.hpp>

#include "rendering/mesh.hpp"
#include "object.hpp"
#include "shader/shaderProgram.hpp"

class PointLight : public Mesh, public Object {
private:
    glm::vec3 colour;

    float intensity;

    float constant;
    float linear;
    float quadratic;
public:
    PointLight(glm::vec3 position, float size, float intensity);

    const glm::vec3& getColour() const;

    void setLightingUniforms(ShaderProgram& shaderProgram, const std::string& i); // Set the uniforms coming from the light source
};