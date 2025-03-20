#include "lighting/lighting.hpp"
#include "utils/constants.hpp"


void Lighting::addPointLight() {
    // If we've reached the maximum number of point lights, remove the oldest one
    if (this->pointLights.size() >= MAX_POINT_LIGHTS) {
        this->pointLights.erase(this->pointLights.begin());
    }
    
    // Get camera properties to define the view frustum
    glm::vec3 cameraPos = camera->getCameraPos();
    glm::vec3 cameraFront = camera->getCameraFront();
    glm::vec3 cameraUp = camera->getGlobalUp();
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    
    // Define frustum parameters
    float near = 1.0f;                              // Near plane distance
    float far = model->getModelRadius() * 3;        // Far plane distance (3x model radius)
    float fov = camera->getFov();                   // Field of view
    float aspectRatio = DEFAULT_ASPECT_RATIO;       // Screen aspect ratio
    
    // Generate random depth within the frustum
    float depth = near + ((float)rand() / RAND_MAX) * (far - near);
    
    // Calculate frustum dimensions at the chosen depth
    float tanHalfFov = glm::tan(glm::radians(fov * 0.5f));
    float halfHeight = depth * tanHalfFov;
    float halfWidth = halfHeight * aspectRatio;
    
    // Generate random position within the frustum slice at the chosen depth
    float randX = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;  // Range [-1, 1]
    float randY = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;  // Range [-1, 1]
    
    // Calculate the final position in world space
    glm::vec3 center = cameraPos + cameraFront * depth;
    glm::vec3 position = center + cameraRight * (randX * halfWidth) + cameraUp * (randY * halfHeight);
    
    // Create and add the new point light
    PointLight pointLight = PointLight(position, DEFAULT_POINT_LIGHT_SIZE);
    this->pointLights.push_back(pointLight);
}


void Lighting::drawPointLights(ShaderProgram& pointLightShader) {
    // Skip rendering if point lights should be hidden
    if (!showPointLights) {
        return;
    }
    
    // Activate shader and set shared uniforms
    pointLightShader.use();
    pointLightShader.setUniform("view", this->view);
    pointLightShader.setUniform("projection", this->projection);
    
    // Draw each point light with its color
    for (PointLight& pointLight : this->pointLights) {
        pointLightShader.setUniform("model", pointLight.getModelMatrix());
        pointLightShader.setUniform("lightColour", pointLight.getColour());
        pointLight.draw();
    }
}


void Lighting::setUniformsForShaderProgram(ShaderProgram& shaderProgram) {
    // Set the number of active point lights
    shaderProgram.setUniform("nr_point_lights", (int) this->pointLights.size());
    
    // Set uniforms for each point light
    for (int i = 0; i < this->pointLights.size(); i++) {
        // Transform the light position to view space for lighting calculations
        glm::vec3 viewSpacePosition = glm::vec3(this->view * glm::vec4(this->pointLights[i].getPosition(), 1.0f));
        shaderProgram.setUniform("pointLights[" + std::to_string(i) + "].position", viewSpacePosition);
        
        // Set other light properties (color, attenuation, etc.)
        pointLights[i].setUniformsForShaderProgram(shaderProgram, std::to_string(i));
    }
    
    // Set directional light uniforms
    this->lightCaster.setUniformsForShaderProgram(shaderProgram);
}