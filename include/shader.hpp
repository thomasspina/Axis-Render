#pragma once

#include <string>
#include <GL/glew.h>

/// @brief OpenGL shader types
enum ShaderType {
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER,
    geometry = GL_GEOMETRY_SHADER,
    tessControl = GL_TESS_CONTROL_SHADER,
    tessEvaluation = GL_TESS_EVALUATION_SHADER
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

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    ~Shader();
};
