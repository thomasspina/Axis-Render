#pragma once

#include "shader.hpp"

#include <unordered_map>
#include <memory>
#include <vector>

class ShaderProgram {
private:
    GLuint programID;
public:
    ShaderProgram();
    ShaderProgram(const Shader& vertShader, const Shader& fragShader);

    void addShader(const Shader& shader);
    void removeShader(GLuint shaderID);

    void use();
    void link();

    GLuint getProgramID() const;

    ~ShaderProgram();
};