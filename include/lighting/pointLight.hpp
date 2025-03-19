#pragma once

#include <glm/glm.hpp>

#include "object.hpp"
#include "shader/shaderProgram.hpp"

class PointLight :  public Object {
private:
    GLuint VAO, VBO, EBO;

    int nIndices;

    glm::vec3 colour;

    float intensity;

    float constant;
    float linear;
    float quadratic;
public:
    PointLight(glm::vec3 position, float size, float intensity);

    const glm::vec3& getColour() const;

    float* getIntensityPointer() { return &intensity; }
    glm::vec3* getColourPointer() { return &colour; }
    glm::vec3 getPosition() const { return position; }

    void updatePosition(const glm::vec3& newPosition) { Object::setPosition(newPosition); }

    void draw();

    void setLightingUniforms(ShaderProgram& shaderProgram, const std::string& i); // Set the uniforms coming from the light source
};