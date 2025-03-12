#include <GL/glew.h>

#include "rendering/worldGrid.hpp"
#include "config.h"

WorldGrid::WorldGrid() {
    ShaderProgram gridShader(std::string(ASSETS_PATH) + "shaders/worldGrid.vert", std::string(ASSETS_PATH) + "shaders/worldGrid.frag");

    // Grid vertices setup
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(glm::vec3), &gridVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gridIndices.size() * sizeof(unsigned int), &gridIndices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

void WorldGrid::draw(const glm::mat4& view, const glm::mat4& projection) {
    gridShader.use();
    gridShader.setUniform("view", view);
    gridShader.setUniform("projection", projection);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, gridIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}