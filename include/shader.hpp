#pragma once



class Shader {
    private:
        void loadShaderFiles(const char* vertexPath, const char* fragmentPath);

    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);
};
