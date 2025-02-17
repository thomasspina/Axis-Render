#pragma once
#include <utility>
#include <GL/glew.h>
#include <string>

class Shader {
    public:
        // Shader program ID
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);
        void use();
        
    private:
        std::pair<std::string, std::string> loadShader(const char* vertexPath, const char* fragmentPath);
        std::pair <unsigned int, unsigned int> compileShader(const char* vShaderCode, const char* fShaderCode);
        void linkShader(unsigned int vertexShader, unsigned int fragmentShader);
};
