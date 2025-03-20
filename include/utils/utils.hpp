#pragma once

#include <vector>
#include <glm/glm.hpp>

inline std::vector<glm::vec3> getCubeVertices(float size) {
    float halfSize = size / 2.0f;
    std::vector<glm::vec3> vertices = {
        // Front face
        {-halfSize, -halfSize, halfSize}, // Bottom-left
        {halfSize, -halfSize, halfSize}, // Bottom-right
        {halfSize, halfSize, halfSize}, // Top-right
        {-halfSize, halfSize, halfSize}, // Top-left
        // Back face
        {-halfSize, -halfSize, -halfSize}, // Bottom-left
        {-halfSize, halfSize, -halfSize}, // Top-left
        {halfSize, halfSize, -halfSize}, // Top-right
        {halfSize, -halfSize, -halfSize}, // Bottom-right
        // Top face
        {-halfSize, halfSize, -halfSize}, // Bottom-left
        {-halfSize, halfSize, halfSize}, // Top-left
        {halfSize, halfSize, halfSize}, // Top-right
        {halfSize, halfSize, -halfSize}, // Bottom-right
        // Bottom face
        {-halfSize, -halfSize, -halfSize}, // Bottom-left
        {halfSize, -halfSize, -halfSize}, // Bottom-right
        {halfSize, -halfSize, halfSize}, // Top-right
        {-halfSize, -halfSize, halfSize}, // Top-left
        // Right face
        {halfSize, -halfSize, -halfSize}, // Bottom-left
        {halfSize, halfSize, -halfSize}, // Top-left
        {halfSize, halfSize, halfSize}, // Top-right
        {halfSize, -halfSize, halfSize}, // Bottom-right
        // Left face
        {-halfSize, -halfSize, -halfSize}, // Bottom-left
        {-halfSize, -halfSize, halfSize}, // Bottom-right
        {-halfSize, halfSize, halfSize}, // Top-right
        {-halfSize, halfSize, -halfSize} // Top-left
    };
    return vertices;
}

inline std::vector<GLuint> getCubeIndices() {
    return {
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