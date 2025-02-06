#pragma once

#include <string>
#include <GL/glew.h>

enum ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragement = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    TessControl = GL_TESS_CONTROL_SHADER,
    TessEvaluation = GL_TESS_EVALUATION_SHADER
};

class Shader {
private:
    ShaderType shaderType;
    GLuint shaderID;

    static const GLchar* readSourceFile(const std::string& sourceFile);
public:
    Shader(ShaderType shaderType);
    Shader(ShaderType shaderType, const std::string& sourceFile);
    
    void loadSource(const std::string& sourceFile);
    void compile();

    GLuint getShaderID() const;
    ShaderType getShaderType() const;

    ~Shader();
};