#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "shader/shader.hpp"

/**
 * @class ShaderProgram
 * @brief Manages an OpenGL shader program and its associated shaders
 * 
 * The ShaderProgram class encapsulates an OpenGL shader program object and provides
 * methods for attaching shaders, linking the program, and setting uniform variables.
 * It supports various data types for uniforms including integers, floats, vectors, and matrices.
 */
class ShaderProgram {
private:
    /** @brief OpenGL program object ID */
    GLuint programID;

public:
    /**
     * @brief Default constructor
     * 
     * Creates an empty shader program without any attached shaders.
     * The program is not linked after creation.
     */
    ShaderProgram();

    /**
     * @brief Constructor with vertex and fragment shaders
     * 
     * Creates a shader program, attaches the provided vertex and fragment shaders,
     * and links the program. The shaders are not deleted afterwards.
     * 
     * @param vertShader Vertex shader to attach
     * @param fragShader Fragment shader to attach
     */
    ShaderProgram(const Shader& vertShader, const Shader& fragShader);

    /**
     * @brief Constructor with vertex and fragment shader file paths
     * 
     * Creates a shader program by loading and compiling vertex and fragment shaders
     * from the specified file paths, attaches them to the program, and links it.
     * The created Shader objects are automatically deleted when they go out of scope.
     * 
     * @param vertShaderPath Path to the vertex shader source file
     * @param fragShaderPath Path to the fragment shader source file
     */
    ShaderProgram(const std::string& vertShaderPath, const std::string& fragShaderPath);

    /**
     * @brief Attaches a shader to the program
     * 
     * @param shader The shader to attach
     */
    void addShader(const Shader& shader);

    /**
     * @brief Detaches a shader from the program
     * 
     * @param shaderID ID of the shader to detach
     */
    void removeShader(GLuint shaderID);

    /**
     * @brief Activates this shader program for rendering
     * 
     * Checks if the program is successfully linked before activation.
     * Outputs an error message to stderr if the program is not linked.
     */
    void use();

    /**
     * @brief Links the shader program
     * 
     * Links all attached shaders into a program. Outputs an error message
     * to stderr if linking fails.
     */
    void link();

    /**
     * @brief Returns the OpenGL program ID
     * 
     * @return GLuint The OpenGL program object ID
     */
    GLuint ID() const;

    /**
     * @brief Sets an integer uniform variable
     * 
     * @param name Name of the uniform variable in the shader
     * @param value Integer value to set
     */
    void setUniform(const std::string& name, GLint value);

    /**
     * @brief Sets an unsigned integer uniform variable
     * 
     * @param name Name of the uniform variable in the shader
     * @param value Unsigned integer value to set
     */
    void setUniform(const std::string& name, GLuint value);

    /**
     * @brief Sets a float uniform variable
     * 
     * @param name Name of the uniform variable in the shader
     * @param value Float value to set
     */
    void setUniform(const std::string& name, GLfloat value);

    /**
     * @brief Sets a 2D vector uniform variable
     * 
     * @param name Name of the uniform variable in the shader
     * @param value 2D vector value to set
     */
    void setUniform(const std::string& name, const glm::vec2& value);

    /**
     * @brief Sets a 3D vector uniform variable
     * 
     * @param name Name of the uniform variable in the shader
     * @param value 3D vector value to set
     */
    void setUniform(const std::string& name, const glm::vec3& value);

    /**
     * @brief Sets a 4D vector uniform variable
     * 
     * @param name Name of the uniform variable in the shader
     * @param value 4D vector value to set
     */
    void setUniform(const std::string& name, const glm::vec4& value);

    /**
     * @brief Sets a 3x3 matrix uniform variable
     * 
     * @param name Name of the uniform variable in the shader
     * @param value 3x3 matrix value to set
     */
    void setUniform(const std::string& name, const glm::mat3& value);

    /**
     * @brief Sets a 4x4 matrix uniform variable
     * 
     * @param name Name of the uniform variable in the shader
     * @param value 4x4 matrix value to set
     */
    void setUniform(const std::string& name, const glm::mat4& value);

    /**
     * @brief Destructor
     * 
     * Deletes the OpenGL shader program.
     */
    ~ShaderProgram();
};