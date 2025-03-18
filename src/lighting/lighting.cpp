#include "lighting/lighting.hpp"
#include "constants.hpp"

void Lighting::addLightCaster(LightCaster lightCaster) {
    this->lightCaster = lightCaster;

    glm::vec3 direction = glm::normalize(lightCaster.getDirection());

    float elevationRad = glm::asin(direction.y);
    float azimuthRad = glm::atan(direction.x, direction.z);
    
    this->elevation = glm::degrees(elevationRad);
    this->azimuth = glm::degrees(azimuthRad);
    
    if (this->azimuth < 0.0f) {
        this->azimuth += 360.0f;
    }
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

glm::vec3 Lighting::anglestoDirection(float azimuth, float elevation) {
    float azimuthRad = glm::radians(azimuth);
    float elevationRad = glm::radians(elevation);
    
    float x = glm::cos(elevationRad) * glm::sin(azimuthRad);
    float y = glm::sin(elevationRad);
    float z = glm::cos(elevationRad) * glm::cos(azimuthRad);
    
    return glm::normalize(glm::vec3(x, y, z));
}

void Lighting::updateCasterDirection() {
    glm::vec3 direction = anglestoDirection(azimuth, elevation);
    this->lightCaster.setDirection(direction);
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

    this->lightCaster.setUniforms(shaderProgram);
}