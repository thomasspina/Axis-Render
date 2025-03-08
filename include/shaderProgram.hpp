#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shader.hpp"


/**
 * Manages a single OpenGL shader program.
 * Handles linking, attaching shaders, and cleanup.
 */
class ShaderProgram {
private:
    GLuint programID;
public:
    ShaderProgram();

    /// @brief  Construct a new Shader Program object with a vertex and fragment shader and link them
    ShaderProgram(const Shader& vertShader, const Shader& fragShader);

    void addShader(const Shader& shader);
    void removeShader(GLuint shaderID);

    void use();
    void link();

    GLuint ID() const;

    void setUniform(const std::string& name, GLint value);
    void setUniform(const std::string& name, GLuint value);
    void setUniform(const std::string& name, GLfloat value);
    void setUniform(const std::string& name, const glm::vec2& value);
    void setUniform(const std::string& name, const glm::vec3& value);
    void setUniform(const std::string& name, const glm::vec4& value);
    void setUniform(const std::string& name, const glm::mat3& value);
    void setUniform(const std::string& name, const glm::mat4& value);

    ~ShaderProgram();
};
