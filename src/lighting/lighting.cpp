#include "lighting/lighting.hpp"
#include "constants.hpp"

void Lighting::addLightCaster(LightCaster lightCaster) {
    this->lightCaster = lightCaster;
}

void Lighting::addPointLight(PointLight pointLight) {
    this->pointLights.push_back(pointLight);
}

void Lighting::updateView(const glm::mat4& view) {
    this->view = view;
}

void Lighting::updateProjection(const glm::mat4& projection) {
    this->projection = projection;
}

void Lighting::drawPointLights(ShaderProgram& pointLightShader) {
    pointLightShader.use();
    pointLightShader.setUniform("view", this->view);
    pointLightShader.setUniform("projection", this->projection);

    for (PointLight& pointLight : this->pointLights) {
        pointLightShader.setUniform("model", pointLight.getModelMatrix());
        pointLightShader.setUniform("lightColour", pointLight.getColour());
        pointLight.draw(pointLightShader);
    }
}

void Lighting::setLightingUniforms(ShaderProgram& shaderProgram) {
    shaderProgram.setUniform("nr_point_lights", (int) this->pointLights.size());
    for (int i = 0; i < this->pointLights.size(); i++) {
        pointLights[i].setLightingUniforms(shaderProgram, std::to_string(i));
    }

    lightCaster.setUniforms(shaderProgram);
}