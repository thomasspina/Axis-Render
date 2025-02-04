#include "shader.hpp"

#include <fstream>
#include <iostream>

const char* Shader::readSourceFile(const std::string& sourceFile) {
    std::ifstream source(sourceFile, std::ios::binary | std::ios::ate);
    if (!source.is_open()) {
        std::cerr << "Error: Failed to open file '" << sourceFile << "'. Please check if the file exists and you have read permissions." << std::endl;
        return nullptr;
    }

    size_t size = source.tellg();
    source.seekg(0, std::ios::beg); // set stream start to beginning of file

    char* buffer = new char[size + 1];

    source.read(buffer, size);
    buffer[size] = '\0'; // null to terminate string
    source.close();

    return buffer;
}

Shader::Shader(ShaderType shaderType) : Shader(shaderType, "") {}

Shader::Shader(ShaderType shaderType, const std::string& sourceFile) {
    this->shaderID = glCreateShader(shaderType);
    this->source = nullptr;

    // set shader source
    if (!sourceFile.empty() && (this->source = readSourceFile(sourceFile))) {
        glShaderSource(shaderID, 1, &this->source, NULL);
        compile();
    }
}

void Shader::loadSource(const std::string& sourceFile) {
    const char* temp;
    if (temp = readSourceFile(sourceFile)) {
        delete[] this->source;
        this->source = temp;
        glShaderSource(shaderID, 1, &this->source, NULL);
    }
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

Shader::~Shader() {
    delete[] this->source;
    glDeleteShader(shaderID);
}