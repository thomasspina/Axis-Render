#include "shader.hpp"

#include <fstream>
#include <iostream>

const GLchar* Shader::readSourceFile(const std::string& sourceFile) {
    std::ifstream source(sourceFile, std::ios::binary | std::ios::ate);
    if (!source.is_open()) {
        std::cerr << "Error: Failed to open file '" << sourceFile << "'. Please check if the file exists and you have read permissions." << std::endl;
        return nullptr;
    }

    size_t size = source.tellg();
    source.seekg(0, std::ios::beg); // set stream start to beginning of file

    GLchar* buffer = new char[size + 1];

    source.read(buffer, size);
    buffer[size] = '\0'; // null to terminate string
    source.close();

    return buffer;
}

Shader::Shader(ShaderType shaderType) : Shader(shaderType, "") {}

Shader::Shader(ShaderType shaderType, const std::string& sourceFile) {
    this->shaderType = shaderType;
    this->shaderID = glCreateShader(shaderType);
    const GLchar* source = nullptr;

    // set shader source
    if (!sourceFile.empty() && (source = readSourceFile(sourceFile))) {
        glShaderSource(shaderID, 1, &source, NULL);
        compile();
    }

    delete[] source;
}

void Shader::loadSource(const std::string& sourceFile) {
    const GLchar* source = nullptr;

    if (source = readSourceFile(sourceFile)) {
        glShaderSource(shaderID, 1, &source, NULL);
    }

    delete[] source;
}

void Shader::compile() {
    GLint success;
    GLchar log[512];

    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, log);
        std::cerr << "Shader compilation failed:\n" << log << std::endl;
    }
}

GLuint Shader::getShaderID() const { return shaderID; }

ShaderType Shader::getShaderType() const { return shaderType; }

Shader::~Shader() {
    glDeleteShader(shaderID);
}