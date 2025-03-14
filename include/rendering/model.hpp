#pragma once

#include <vector>
#include <string>
#include <assimp/scene.h>

#include "rendering/mesh.hpp"
#include "object.hpp"
#include <memory>
#include <assimp/Importer.hpp>

/*
Obj file info:

.obj:
mtllib model.mtl: link to MTL file
usemtl ModelMaterial : Apply ModelMaterial

v 1.0 0.0 0.0 : vertex position (x ,y ,z)
vt 0.0 0.0: texture coordinate (U, V)
vn 0.0 0.0 1.0: Normal (x, y, z)
f 1/1/1 2/2/1 3/3/1: Face, which is triangular in our case (v/vt/vn for each vertex) 

.mtl:
newmtl ModelMaterial: New material called ModelMaterial
Ka 1.0 1.0 1.0        # Ambient color (RGB)
Kd 1.0 1.0 1.0        # Diffuse color (RGB)
Ks 0.5 0.5 0.5        # Specular color (RGB)
Ns 50.0               # Shininess (higher = shinier)
d 1.0                 # Opacity (1.0 = fully visible, 0.0 = fully transparent)
illum 2               # Illumination model (lighting type)
map_Kd texture.png    # Diffuse texture (Base color)
map_Ks specular.png   # Specular texture (Shininess Map)
map_Bump normal.png   # Normal map (bump mapping), encodes surface bumps
*/

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

        unsigned int applyNullTexture();

    public:
        Model(const std::string &path);

        ~Model();

        void cleanup();

        void calculateModelDimension();

        void draw(ShaderProgram &shader);

        const glm::vec3 getModelCenter() const;

        const float getModelRadius() const;
};
