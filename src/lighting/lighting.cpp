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

        // Transform the light position to view space
        glm::vec3 viewSpacePosition = glm::vec3(this->view * glm::vec4(this->pointLights[i].getPosition(), 1.0f));
        shaderProgram.setUniform("pointLights[" + std::to_string(i) + "].position", viewSpacePosition);


        pointLights[i].setLightingUniforms(shaderProgram, std::to_string(i));
    }

    lightCaster.setUniforms(shaderProgram);
}