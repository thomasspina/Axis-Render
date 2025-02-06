#pragma once

#include <string>
#include <GL/glew.h>

/// @brief OpenGL shader types
enum ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragement = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    TessControl = GL_TESS_CONTROL_SHADER,
    TessEvaluation = GL_TESS_EVALUATION_SHADER
};
/**
 * Manages an individual OpenGL shader object (vert, frag, geom, etc)
 * Handles loading from file. compilation and cleanup. 
 * File paths are relative to the assets folder. 
 */
class Shader {
private:
    ShaderType shaderType;
    GLuint shaderID;

    static const GLchar* readSourceFile(const std::string& sourceFile);
public:
    
    /// @brief creates a Shader object. loads the source code, and compiles it
    Shader(ShaderType shaderType, const std::string& sourceFile);
    Shader(ShaderType shaderType);
    
    /// @brief only loads the new source code without compiling it
    void loadSource(const std::string& sourceFile);
    void compile();

    GLuint getShaderID() const;
    ShaderType getShaderType() const;

    ~Shader();
};