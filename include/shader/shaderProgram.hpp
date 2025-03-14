#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shader/shader.hpp"


/**
 * Manages a single OpenGL shader program.
 * Handles linking, attaching shaders, and cleanup.
 */
class ShaderProgram {
private:
    GLuint programID;
public:
    ShaderProgram();

    /// @brief  Construct a new Shader Program object with a vertex and fragment shader and link them. Doesn't delete the shaders after
    ShaderProgram(const Shader& vertShader, const Shader& fragShader);

    /// @brief Construct a new Shader Program object with a vertex and fragment shader source file path and link them. Deletes the shaders after
    ShaderProgram(const std::string& vertShaderPath, const std::string& fragShaderPath);

    void addShader(const Shader& shader);
    void removeShader(GLuint shaderID);

    void use();
    void link();

    GLuint ID() const;

    // Sets an integer uniform variable in the shader program
    void setUniform(const std::string& name, GLint value);

    // Sets an unsigned integer uniform variable in the shader program
    void setUniform(const std::string& name, GLuint value);

    // Sets a floating-point uniform variable in the shader program
    void setUniform(const std::string& name, GLfloat value);

    // Sets a vec2 uniform variable in the shader program
    void setUniform(const std::string& name, const glm::vec2& value);

    // Sets a vec3 uniform variable in the shader program
    void setUniform(const std::string& name, const glm::vec3& value);

    // Sets a vec4 uniform variable in the shader program
    void setUniform(const std::string& name, const glm::vec4& value);

    // Sets a mat3 uniform variable in the shader program
    void setUniform(const std::string& name, const glm::mat3& value);

    // Sets a mat4 uniform variable in the shader program
    void setUniform(const std::string& name, const glm::mat4& value);

    ~ShaderProgram();
};
