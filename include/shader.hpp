#pragma once



class Shader {
    private:
        void loadShaderFiles();
        void compileShader();


    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);



};
