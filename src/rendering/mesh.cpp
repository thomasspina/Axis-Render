#include <GL/glew.h>

#include "rendering/mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, float shininess) 
    : Mesh(vertices, indices, textures) 
{
    this->shininess = shininess;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    for (const auto& texture : textures) {
        glDeleteTextures(1, &texture.id);
    }
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Store vertex data into the currently bounded VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Store indice data into the currently bounded EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Set vertex attributes pointers which tells OpenGL how it should interpret vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Set normals attribute pointers
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Set texture attribute pointers
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0); // Unbind VAO
}

void Mesh::draw(ShaderProgram& shaderProgram) {
    GLuint diffuseNr = 0;
    GLuint specularNr = 0;

    for (GLuint i = 0; i < textures.size(); i++) {

        // Shift to next texture unit macro
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;

        if (name == "texture_diffuse") {
            shaderProgram.setUniform("material.texture_diffuse[" + number + "]", i);
            number = std::to_string(diffuseNr++);
        } else if (name == "texture_specular") {
            shaderProgram.setUniform("material.texture_specular[" + number + "]", i);
            number = std::to_string(specularNr++);
        }

        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    shaderProgram.setUniform("material.diffuse_count", diffuseNr);
    shaderProgram.setUniform("material.specular_count", specularNr);
    shaderProgram.setUniform("shininess", shininess);
    
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // reset to default texture
    glActiveTexture(GL_TEXTURE0);
}



