#include "shaderProgram.hpp"

#include <iostream>

ShaderProgram::ShaderProgram() {
    programID = glCreateProgram();
}

ShaderProgram::ShaderProgram(const Shader& vertShader, const Shader& fragShader) {
    programID = glCreateProgram();
    addShader(vertShader);
    addShader(fragShader);
    link();
}

void ShaderProgram::addShader(const Shader& shader) {
    glAttachShader(programID, shader.getShaderID());
}

void ShaderProgram::removeShader(GLuint shaderID) {
    glDetachShader(programID, shaderID);
}

void ShaderProgram::use() {
    int success;
    char log[512];

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, log);
        std::cerr << "ERROR::SHADER::PROGRAM::NOT_LINKED\n" << log << std::endl;
        return;
    }

    glUseProgram(programID);
}

void ShaderProgram::link() {
    glLinkProgram(programID);

    int success;
    char log[512];

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
    }
}

GLuint ShaderProgram::ID() const { return programID; }

// Update uniform variables of type integer
void ShaderProgram::setUniform(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

// Update uniform variables of type float
void ShaderProgram::setUniform(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

// Update uniform variables of type vec2
void ShaderProgram::setUniform(const std::string& name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

// Update uniform variables of type vec3
void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

// Update uniform variables of type vec4
void ShaderProgram::setUniform(const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);
}
