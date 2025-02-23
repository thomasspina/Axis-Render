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

GLuint ShaderProgram::getProgramID() const { return programID; }

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);
}