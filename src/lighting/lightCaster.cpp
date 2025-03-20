#include "lighting/lightCaster.hpp"

/*****************************************/
/*            Public Methods            */
/*****************************************/

LightCaster::LightCaster(float azimuth, float elevation) {
    this->azimuth = azimuth;
    this->elevation = elevation;
    this->intensity = DEFAULT_LIGHT_INTENSITY;
    this->colour = DEFAULT_LIGHT_COLOUR;
    this->direction = anglestoDirection(azimuth, elevation);
}

LightCaster::LightCaster(const glm::vec3& direction) {
    this->direction = glm::normalize(direction);
    this->colour = DEFAULT_LIGHT_COLOUR;
    this->intensity = DEFAULT_LIGHT_INTENSITY;
    
    // Calculate equivalent angular coordinates from the direction vector
    float elevationRad = glm::asin(direction.y);
    float azimuthRad = glm::atan(direction.x, direction.z);
    this->elevation = glm::degrees(elevationRad);
    this->azimuth = glm::degrees(azimuthRad);
    
    // Ensure azimuth is in the range [0, 360)
    if (this->azimuth < 0.0f) {
        this->azimuth += 360.0f;
    }
}

void LightCaster::setDirection(const glm::vec3& newDirection) {
    this->direction = glm::normalize(newDirection);
    
    // Calculate equivalent angular coordinates from the direction vector
    std::pair<float, float> angles = directionToAngles(newDirection);
    this->azimuth = angles.first;
    this->elevation = angles.second;
}

void LightCaster::setDirection(float azimuth, float elevation) {
    this->azimuth = azimuth;
    this->elevation = elevation;
    this->direction = anglestoDirection(azimuth, elevation);
}


/*****************************************/
/*            Private Methods            */
/*****************************************/


void LightCaster::setUniformsForShaderProgram(ShaderProgram& shaderProgram) {
    shaderProgram.setUniform("dirLight.direction", this->direction);
    
    // Calculate light components using intensity and default multipliers
    glm::vec3 ambient = this->colour * DEFAULT_LIGHT_AMBIENT * this->intensity;
    glm::vec3 diffuse = this->colour * this->intensity * DEFAULT_LIGHT_DIFFUSE;
    glm::vec3 specular = this->colour * this->intensity * DEFAULT_LIGHT_SPECULAR;
    
    // Set shader uniforms
    shaderProgram.setUniform("dirLight.ambient", ambient);
    shaderProgram.setUniform("dirLight.diffuse", diffuse);
    shaderProgram.setUniform("dirLight.specular", specular);
}

glm::vec3 LightCaster::anglestoDirection(float azimuth, float elevation) {
    // Convert angles from degrees to radians
    float azimuthRad = glm::radians(azimuth);
    float elevationRad = glm::radians(elevation);
    
    // Calculate direction vector components using spherical coordinates
    float x = glm::cos(elevationRad) * glm::sin(azimuthRad);
    float y = glm::sin(elevationRad);
    float z = glm::cos(elevationRad) * glm::cos(azimuthRad);
    
    return glm::normalize(glm::vec3(x, y, z));
}

std::pair<float, float> LightCaster::directionToAngles(const glm::vec3& direction) {
    // Calculate azimuth angle
    float azimuth = glm::degrees(glm::atan(direction.x, direction.z));
    
    // Calculate elevation angle
    float elevation = glm::degrees(glm::asin(direction.y));
    
    return std::make_pair(azimuth, elevation);
}