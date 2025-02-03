#include "shader.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <GL/gl.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

}

std::pair<const char*, const char*> Shader::loadShader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::stringstream vShaderStream;
    std::stringstream fShaderStream;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        // Read shader file's buffer into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        // Convert stream contents into strings
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    } catch(std::ifstream::failure e) {
        std::cerr << "Failed to read shader files!" << std::endl;
    }

    // OpenGL only accepts c string
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    return {vShaderCode, fShaderCode};
}

void compileShader(const char* vShaderCode, const char* fShaderCode) {
    unsigned int vertexShader, fragmentShader;
    int isSuccess;
    char infoLog[512];

    // Create a shader object and get its unique ID
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach shader code to the shader object
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);

    // Compile shader code into machine code for the GPU
    glCompileShader(vertexShader);

    // Get compile status info
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isSuccess);

    if (!isSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Failed to compile Vertex Shader!\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isSuccess);

    if (!isSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Failed to compile Fragment Shader!\n" << infoLog << std::endl;
    }
}

