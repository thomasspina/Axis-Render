#pragma once

#include "shader.hpp"

#include <unordered_map>
#include <memory>

class ShaderProgram {
private:
    GLuint programID;
    std::unordered_map<ShaderType, std::shared_ptr<Shader>> shaders;
public:
    ShaderProgram();
    ~ShaderProgram();

    bool addShader(const Shader& shader);
};