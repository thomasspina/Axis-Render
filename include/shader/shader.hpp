#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

/**
 * @enum ShaderType
 * @brief Enumeration of supported GLSL shader types
 * 
 * Maps the different shader stages in the OpenGL rendering pipeline
 * to their corresponding OpenGL enum values.
 */
enum ShaderType {
    vertex = GL_VERTEX_SHADER,           ///< Vertex shader stage
    fragment = GL_FRAGMENT_SHADER,       ///< Fragment (pixel) shader stage
    geometry = GL_GEOMETRY_SHADER,       ///< Geometry shader stage
    tessControl = GL_TESS_CONTROL_SHADER,    ///< Tessellation control shader stage
    tessEvaluation = GL_TESS_EVALUATION_SHADER ///< Tessellation evaluation shader stage
};

/**
 * @class Shader
 * @brief Represents and manages a single GLSL shader object
 * 
 * The Shader class encapsulates the functionality for loading, compiling,
 * and managing a shader of any supported type (vertex, fragment, etc.).
 * It handles loading shader source from files.
 */
class Shader {
private:
    ShaderType shaderType;  ///< Type of this shader (vertex, fragment, etc.)
    GLuint shaderID;        ///< OpenGL shader object ID

    /**
     * @brief Reads shader source code from a file
     * 
     * Loads the entire content of a shader source file into memory,
     * adding a null terminator at the end.
     * 
     * @param sourceFile Path to the shader source file
     * @return GLchar* Pointer to null-terminated string containing shader source
     */
    static const GLchar* readSourceFile(const std::string& sourceFile);

public:
    /**
     * @brief Constructs a shader of the specified type
     * 
     * Creates an empty shader without loading source code.
     * 
     * @param shaderType Type of shader to create
     */
    Shader(ShaderType shaderType) : Shader(shaderType, "") {}

    /**
     * @brief Constructs a shader of the specified type with source from a file
     * 
     * Creates a shader and immediately loads and compiles source code if provided.
     * 
     * @param shaderType Type of shader to create
     * @param sourceFile Path to the shader source file (can be empty)
     */
    Shader(ShaderType shaderType, const std::string& sourceFile);

    /**
     * @brief Loads shader source code from a file
     * 
     * Replaces any existing shader source with the contents of the specified file.
     * 
     * @param sourceFile Path to the shader source file
     */
    void loadSource(const std::string& sourceFile);

    /**
     * @brief Compiles the loaded shader source code
     * 
     * Attempts to compile the shader and outputs any errors to the console.
     */
    void compile();

    /**
     * @brief Gets the OpenGL shader object ID
     * @return GLuint OpenGL shader ID
     */
    GLuint getShaderID() const { return shaderID; }

    /**
     * @brief Gets the type of this shader
     * @return ShaderType Type of this shader
     */
    ShaderType getShaderType() const { return shaderType; }

    /**
     * @brief Destructor - cleans up OpenGL resources
     * 
     * Deletes the shader object from the OpenGL context.
     */
    ~Shader();
};