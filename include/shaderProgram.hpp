#pragma once

#include "shader.hpp"

#include <unordered_map>
#include <memory>
#include <vector>
#include <glm/glm.hpp>


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

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const glm::vec2& value);
    void setUniform(const std::string& name, const glm::vec3& value);
    void setUniform(const std::string& name, const glm::vec4& value);

    ~ShaderProgram();
};
