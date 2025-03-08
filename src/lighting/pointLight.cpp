#include <string>

#include "lighting/pointLight.hpp"
#include "lighting/utils.hpp"
#include "config.h"


PointLight::PointLight(glm::vec3 position, float size, float intensity) : Mesh(getCubeVertices(size), getCubeIndices(), std::vector<Texture>()) {
    this->position = position;
    this->colour = glm::vec3(1.0f);
    this->intensity = intensity;
    this->constant = 1.0f;
    this->linear = 0.09f;
    this->quadratic = 0.032f;

    Shader vertexShader = Shader(vertex, std::string(ASSETS_PATH) + "shaders/pointLight.vert");
    Shader fragmentShader = Shader(fragment, std::string(ASSETS_PATH) + "shaders/pointLight.frag");
    this->shaderProgram = ShaderProgram(vertexShader, fragmentShader);
}

PointLight::~PointLight() {
    this->shaderProgram.~ShaderProgram();
}

void PointLight::setUniforms(ShaderProgram& shaderProgram, const std::string& i) {
    shaderProgram.setUniform("pointLights[" + i + "].position", this->position);

    shaderProgram.setUniform("pointLights[" + i + "].ambient", this->colour);
    shaderProgram.setUniform("pointLights[" + i + "].diffuse", this->colour);
    shaderProgram.setUniform("pointLights[" + i + "].specular", this->colour);

    glm::vec3 ambient = this->colour * 0.1f * this->intensity; 
    
    glm::vec3 diffuse = this->colour * this->intensity; 
    
    // white-tinted version of the light color
    glm::vec3 specular = glm::mix(this->colour, glm::vec3(1.0f), 0.5f) * this->intensity;
    
    // Set the uniforms
    shaderProgram.setUniform("pointLights[" + i + "].ambient", ambient);
    shaderProgram.setUniform("pointLights[" + i + "].diffuse", diffuse);
    shaderProgram.setUniform("pointLights[" + i + "].specular", specular);
}

void PointLight::draw(ShaderProgram& shader) {
    Mesh::draw(this->shaderProgram);
}