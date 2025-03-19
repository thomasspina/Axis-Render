#include <string>

#include "lighting/pointLight.hpp"
#include "lighting/utils.hpp"
#include "constants.hpp"


PointLight::PointLight(glm::vec3 position, float size, float intensity) : Object(position) {
    this->colour = DEFAULT_LIGHT_COLOUR;
    this->intensity = intensity;
    this->constant = DEFAULT_LIGHT_CONSTANT;
    this->linear = DEFAULT_LIGHT_LINEAR;
    this->quadratic = DEFAULT_LIGHT_QUADRATIC;


    // Setup pointlight mesh
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    std::vector<glm::vec3> vertices = getCubeVertices(size);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    std::vector<GLuint> indices = getCubeIndices();
    nIndices = indices.size();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glBindVertexArray(0); 
}

const glm::vec3& PointLight::getColour() const {
    return colour;
}

void PointLight::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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