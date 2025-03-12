#pragma once

#include <vector>
#include <string>
#include <assimp/scene.h>

#include "mesh.hpp"
#include "object.hpp"
#include <memory>
#include <assimp/Importer.hpp>


class Model : public Object {
    private:
        Assimp::Importer import;
        Assimp::Importer importWithoutFlip; 
        const aiScene* scene;
        
        bool needFlip = false;
        glm::vec3 modelSize;
        glm::vec3 modelCenter;
        float modelRadius;

        glm::vec3 minBounds;
        glm::vec3 maxBounds;

        std::vector<Texture> textures_loaded;
        std::vector<std::unique_ptr<Mesh>> meshes;
        std::string directory;

        void loadModel(const std::string &path);

        bool shouldFlipModel(const aiScene* scene);
        bool meshRequireFlip(const aiMesh* mesh);

        void processNode(aiNode *node, const aiScene *scene);
        std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        unsigned int textureFromFile(const std::string &path, const std::string &directory, bool gamma = false);

    public:
        Model(const std::string &path);

        ~Model();

        void cleanup();

        void calculateModelDimension();

        void draw(ShaderProgram &shader);

        const glm::vec3 getModelCenter() const;

        const float getModelRadius() const;
};
