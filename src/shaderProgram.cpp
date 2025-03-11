#include <iostream>

#include "shaderProgram.hpp"

ShaderProgram::ShaderProgram() {
    programID = glCreateProgram();
}

ShaderProgram::ShaderProgram(const Shader& vertShader, const Shader& fragShader) {
    programID = glCreateProgram();
    addShader(vertShader);
    addShader(fragShader);
    link();
}

ShaderProgram::ShaderProgram(const std::string& vertShaderPath, const std::string& fragShaderPath) {
    // create shaders
    Shader vertShader = Shader(vertex, vertShaderPath);
    Shader fragShader = Shader(fragment, fragShaderPath);

    programID = glCreateProgram();
    addShader(vertShader);
    addShader(fragShader);
    link();

    // shaders are deleted since Shader class destructor deletes them
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

void ShaderProgram::setUniform(const std::string& name, GLint value) {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& name, GLuint value) {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& name, GLfloat value) {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);
}
