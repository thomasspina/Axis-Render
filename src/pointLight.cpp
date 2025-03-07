#include "pointLight.hpp"
#include "tools.hpp"
#include "config.h"


PointLight::PointLight(glm::vec3 position, glm::vec3 colour, float intensity, float constant, float linear, float quadratic): mesh(createCubeMesh(0.1f))
{
    this->position = position;
    this->colour = colour;
    this->intensity = intensity;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

    // this->mesh = new Mesh(createCubeMesh(0.1f)); // TODO: add dynamic size

    Shader vertexShader = Shader(vertex, std::string(ASSETS_PATH) + "shaders/pointLight.vert");
    Shader fragmentShader = Shader(fragment, std::string(ASSETS_PATH) + "shaders/pointLight.frag");
    this->shaderProgram = ShaderProgram(vertexShader, fragmentShader);
}

PointLight::~PointLight() {
    this->shaderProgram.~ShaderProgram();
}

