#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>


#include "shader.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
    private:
        // Vertex Array Object: Manages vertex attribute configurations and bindings to VBOs and EBOs.
        // Vertex Buffer Object: Stores vertex data like positions, normals, and texture coordinates in GPU memory.
        // Element Buffer Object: Holds indices for indexed drawing to optimize memory usage and performance.    
        unsigned VAO, VBO, EBO;

        void setupMesh();

    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        void draw(Shader& shader);
};
