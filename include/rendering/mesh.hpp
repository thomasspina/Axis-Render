#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "shader/shaderProgram.hpp"

/**
 * @struct Vertex
 * @brief Vertex data structure for the Mesh class.
 * 
 * Represents a vertex in 3D space, containing the position, normal vector, and texture coordinates.
 * This structure is used to store information about each vertex for the mesh.
 */
struct Vertex {
    glm::vec3 position; ///< The 3D position of the vertex.
    glm::vec3 normal; ///< The normal vector at the vertex.
    glm::vec2 texCoords; ///< The texture coordinates of the vertex.
};

/**
 * @struct Texture
 * @brief Texture data structure for the Mesh class.
 * 
 * Represents a texture applied to the mesh. Contains the OpenGL texture ID, the type of the texture
 * (e.g., diffuse, specular), and the file path to the texture image.
 */
struct Texture {
    GLuint id; ///< The OpenGL ID of the texture.
    std::string type; ///< The type of the texture (e.g., "diffuse", "specular").
    std::string path; ///< The file path to the texture image.
};

/**
 * @class Mesh
 * @brief Manages a mesh object.
 * 
 * Handles rendering and cleanup of a 3D mesh, including managing vertex data, textures, and drawing the mesh 
 * with appropriate shaders. It also sets up the necessary OpenGL buffers (VAO, VBO, EBO) for rendering.
 */
class Mesh {
    private:
        /**
         * @brief Vertex Array Object (VAO).
         * 
         * Manages vertex attribute configurations and binds VBOs and EBOs for rendering.
         */
        GLuint VAO;

        /**
         * @brief Vertex Buffer Object (VBO).
         * 
         * Stores vertex data (positions, normals, and texture coordinates) in GPU memory for efficient access.
         */
        GLuint VBO;

        /**
         * @brief Element Buffer Object (EBO).
         * 
         * Holds indices for indexed drawing, optimizing memory usage and performance during rendering.
         */
        GLuint EBO;

        std::vector<Vertex> vertices; ///< A vector containing the mesh's vertex data.
        std::vector<GLuint> indices; ///< A vector containing the mesh's index data.
        std::vector<Texture> textures; ///< A vector containing the textures applied to the mesh.

        float shininess = 32.0f; ///< The shininess of the material applied to the mesh.

        /**
         * @brief Initializes the VAO, VBO, and EBO for the mesh.
         * 
         * This method sets up OpenGL buffers and vertex attribute pointers for positions, normals, 
         * and texture coordinates, allowing the mesh to be rendered efficiently.
         */
        void setupMesh();

    public:
        /**
         * @brief Constructs a Mesh object with vertex data, index data, and textures.
         * 
         * Initializes the mesh and sets up the necessary OpenGL buffers for efficient rendering.
         * 
         * @param vertices A vector containing the vertex data (positions, normals, texCoords).
         * @param indices A vector containing the index data for indexed drawing.
         * @param textures A vector containing the textures applied to the mesh.
         */
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

        /**
         * @brief Constructs a Mesh object with vertex data, index data, textures, and shininess.
         * 
         * Initializes the mesh, sets the shininess property, and sets up the necessary OpenGL buffers.
         * 
         * @param vertices A vector containing the vertex data (positions, normals, texCoords).
         * @param indices A vector containing the index data for indexed drawing.
         * @param textures A vector containing the textures applied to the mesh.
         * @param shininess The shininess of the material.
         */
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, float shininess);

        /**
         * @brief Destructor for the Mesh class.
         * 
         * Cleans up the OpenGL resources by deleting the VAO, VBO, EBO, and textures associated with the mesh.
         */
        ~Mesh();

        /**
         * @brief Draws the mesh using the provided shader program.
         * 
         * This method binds the appropriate textures and sets shader uniforms for material properties 
         * like diffuse and specular textures, shininess, and texture counts. It then draws the mesh 
         * using OpenGL's `glDrawElements` function.
         * 
         * @param shader The shader program used for rendering the mesh.
         */
        void draw(ShaderProgram& shader);
};
