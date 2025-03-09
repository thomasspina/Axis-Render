#include "lighting/lightCaster.hpp"
#include "constants.hpp"

#include <iostream>

LightCaster::LightCaster(const glm::vec3& direction, float intensity) {
    this->direction = direction;
    this->colour = glm::vec3(1.0f, 1.0f, 1.0f);
    this->intensity = intensity;
}

void printVec3(glm::vec3 vec) {
    std::cout << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z << std::endl;
}

void LightCaster::setUniforms(ShaderProgram& shaderProgram) {
    shaderProgram.setUniform("dirLight.direction", this->direction);

    glm::vec3 ambient = this->colour * DEFAULT_LIGHT_AMBIENT * this->intensity;
    glm::vec3 diffuse = this->colour * this->intensity * DEFAULT_LIGHT_DIFFUSE;
    glm::vec3 specular = this->colour * this->intensity * DEFAULT_LIGHT_SPECULAR;

    // printVec3(ambient);
    // printVec3(diffuse);
    // printVec3(specular);

    shaderProgram.setUniform("dirLight.ambient", ambient);
    shaderProgram.setUniform("dirLight.diffuse", diffuse);
    shaderProgram.setUniform("dirLight.specular", specular);
}