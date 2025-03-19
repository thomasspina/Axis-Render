#include <string>

#include "lighting/pointLight.hpp"
#include "lighting/utils.hpp"
#include "constants.hpp"


PointLight::PointLight(glm::vec3 position, float size, float intensity) : Mesh(getCubeVertices(size), getCubeIndices(), {}), Object(position) {
    this->colour = DEFAULT_LIGHT_COLOUR;
    this->intensity = intensity;
    this->constant = DEFAULT_LIGHT_CONSTANT;
    this->linear = DEFAULT_LIGHT_LINEAR;
    this->quadratic = DEFAULT_LIGHT_QUADRATIC;
}

const glm::vec3& PointLight::getColour() const {
    return colour;
}

void PointLight::setLightingUniforms(ShaderProgram& shaderProgram, const std::string& i) {
    glm::vec3 ambient = this->colour * DEFAULT_LIGHT_AMBIENT * this->intensity; 
    
    glm::vec3 diffuse = this->colour * this->intensity * DEFAULT_LIGHT_DIFFUSE; 
    
    // white-tinted version of the light color
    glm::vec3 specular = glm::mix(this->colour, glm::vec3(1.0f), 0.5f) * this->intensity * DEFAULT_LIGHT_SPECULAR;
    
    // Set the uniforms
    shaderProgram.setUniform("pointLights[" + i + "].ambient", ambient);
    shaderProgram.setUniform("pointLights[" + i + "].diffuse", diffuse);
    shaderProgram.setUniform("pointLights[" + i + "].specular", specular);

    shaderProgram.setUniform("pointLights[" + i + "].constant", this->constant);
    shaderProgram.setUniform("pointLights[" + i + "].linear", this->linear);
    shaderProgram.setUniform("pointLights[" + i + "].quadratic", this->quadratic);
}