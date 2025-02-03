#pragma once
#include <utility>
#include <GL/glew.h>

class Shader {
    private:
        std::pair<const char*, const char*> loadShader(const char* vertexPath, const char* fragmentPath);
        void compileShader(const char* vShaderCode, const char* fShaderCode);
        void linkShader();

    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);
};
