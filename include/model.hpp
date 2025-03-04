#pragma once

#include "mesh.hpp"
#include <vector>
#include <string>
#include <assimp/scene.h>

class Model {
    private:
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;
        void loadModel(const std::string &path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        unsigned int textureFromFile(const std::string &path, const std::string &directory, bool gamma = false);

    public:
        Model(const std::string &path);

        void draw(ShaderProgram &shader);
};
