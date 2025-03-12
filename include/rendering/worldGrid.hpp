#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shader/shaderProgram.hpp"

class WorldGrid {
private:
    const std::vector<glm::vec3> gridVertices = {
        glm::vec3(-1.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, 1.0f)
    };

    const std::vector<unsigned int> gridIndices = {
        0, 1, 2,
        2, 3, 0
    };

    ShaderProgram gridShader;
    unsigned int VAO, VBO, EBO;
public:
    WorldGrid();

    void draw(const glm::mat4& view, const glm::mat4& projection);
};