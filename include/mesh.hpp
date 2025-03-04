#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

// #include "shader.hpp"
#include "shaderProgram.hpp"

/// @brief Vertex data structure for the Mesh class
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

/// @brief Texture data structure for the Mesh class
struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

/**
 * Manages a mesh object.
 * Handles rendering and cleanup.
 */
class Mesh {
    private:
        // Vertex Array Object: Manages vertex attribute configurations and bindings to VBOs and EBOs.
        // Vertex Buffer Object: Stores vertex data like positions, normals, and texture coordinates in GPU memory.
        // Element Buffer Object: Holds indices for indexed drawing to optimize memory usage and performance.    
        GLuint VAO, VBO, EBO;

        void setupMesh();

    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

        void draw(ShaderProgram& shader);
};
