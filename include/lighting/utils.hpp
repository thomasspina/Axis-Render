#include <vector>
#include <glm/glm.hpp>

#include "mesh.hpp"

std::vector<Vertex> getCubeVertices(float size) {
    float halfSize = size / 2.0f;
    
    std::vector<Vertex> vertices = {
        // Front face
        {{-halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // Top-right
        {{-halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // Top-left
        
        // Back face
        {{-halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}}, // Bottom-left
        {{-halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}}, // Top-left
        {{ halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}}, // Top-right
        {{ halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}}, // Bottom-right
        
        // Top face
        {{-halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{-halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left
        {{ halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}, // Top-right
        {{ halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
        
        // Bottom face
        {{-halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // Bottom-left
        {{ halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}, // Bottom-right
        {{ halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}, // Top-right
        {{-halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}, // Top-left
        
        // Right face
        {{ halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ halfSize,  halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left
        {{ halfSize,  halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // Top-right
        {{ halfSize, -halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
        
        // Left face
        // front 
        0, 1, 2,
        2, 3, 0,
        
        // back 
        4, 5, 6,
        6, 7, 4,
        
        // top 
        8, 9, 10,
        10, 11, 8,
        
        // bottom 
        12, 13, 14,
        14, 15, 12,
        
        // right 
        16, 17, 18,
        18, 19, 16,
        
        // left 
        20, 21, 22,
        22, 23, 20
    };
}