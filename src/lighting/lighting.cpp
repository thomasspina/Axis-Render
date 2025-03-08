#include "lighting/lighting.hpp"
#include "config.h"

Lighting::Lighting() {
    Shader pointLightVertexShader = Shader(vertex, std::string(ASSETS_PATH) + "shaders/pointLight.vert");
    Shader pointLightFragmentShader = Shader(fragment, std::string(ASSETS_PATH) + "shaders/pointLight.frag");
    this->pointLightShader = ShaderProgram(pointLightVertexShader, pointLightFragmentShader);
}

Lighting::~Lighting() {
    this->pointLightShader.~ShaderProgram();
}