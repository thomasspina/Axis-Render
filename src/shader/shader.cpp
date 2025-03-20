#include <fstream>
#include <iostream>

#include "shader/shader.hpp"


/*****************************************/
/*            Public Methods             */
/*****************************************/


Shader::Shader(ShaderType shaderType, const std::string& sourceFile) {
    this->shaderType = shaderType;
    this->shaderID = glCreateShader(shaderType);
    const GLchar* source = nullptr;
    
    // Load and compile source if a file was specified
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
    
    // Attempt to compile the shader
    glCompileShader(shaderID);
    
    // Check for compilation errors
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, log);
        std::cerr << "Shader compilation failed:\n" << log << std::endl;
    }
}


Shader::~Shader() {
    glDeleteShader(shaderID);
}



/*****************************************/
/*            Private Methods            */
/*****************************************/


const GLchar* Shader::readSourceFile(const std::string& sourceFile) {
    // Open the file in binary mode at the end to determine size
    std::ifstream source(sourceFile, std::ios::binary | std::ios::ate);
    if (!source.is_open()) {
        std::cerr << "Error: Failed to open file '" << sourceFile << "'. Please check if the file exists and you have read permissions." << std::endl;
        return nullptr;
    }
    
    // Get the file size and allocate a buffer
    size_t size = source.tellg(); // Get the shader file size
    source.seekg(0, std::ios::beg); // Set stream position to beginning of file
    GLchar* buffer = new char[size + 1];
    
    // Read the entire file into the buffer
    source.read(buffer, size);
    buffer[size] = '\0'; // Null-terminate the string
    source.close();
    
    return buffer;
}