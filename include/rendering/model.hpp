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

/**
 * @class Model
 * @brief Represents a 3D model loaded from an OBJ file
 * 
 * The Model class is responsible for loading and storing a 3D model, processing its meshes, 
 * applying materials and textures, and calculating model dimensions. It handles loading 
 * geometry, normals, texture coordinates, and materials through the Assimp library.
 */
class Model : public Object {
private:
    Assimp::Importer import;                ///< Assimp importer for model loading with texture flipping
    Assimp::Importer importWithoutFlip;      ///< Assimp importer for model loading without texture flipping
    const aiScene* scene;                   ///< Assimp scene object containing the loaded model data

    bool needFlip = false;                  ///< Flag indicating whether model textures need to be flipped
    glm::vec3 modelSize;                    ///< The size of the model (width, height, depth)
    glm::vec3 modelCenter;                  ///< The center of the model's bounding box
    float modelRadius;                      ///< The radius of the model's bounding sphere

    glm::vec3 minBounds;                    ///< The minimum bounds (corner) of the model
    glm::vec3 maxBounds;                    ///< The maximum bounds (corner) of the model

    std::vector<Texture> textures_loaded;    ///< A list of textures that have been loaded
    std::vector<std::unique_ptr<Mesh>> meshes; ///< A list of meshes that make up the model
    std::string directory;                   ///< Directory path of the model for texture loading

    /**
     * @brief Loads a 3D model from a file
     * 
     * This function uses the Assimp library to load the model and its associated meshes and textures.
     * The function also checks whether texture flipping is required based on the model's UV mapping.
     * 
     * @param path The path to the model file
     */
    void loadModel(const std::string &path);

    /**
     * @brief Determines whether the model requires flipping of texture coordinates
     * 
     * The function checks if more than half of the model's meshes require flipping of their UVs.
     * 
     * @param scene The Assimp scene object
     * @return True if the model requires texture flipping, false otherwise
     */
    bool shouldFlipModel(const aiScene* scene);

    /**
     * @brief Checks if the given mesh requires flipping of texture coordinates
     * 
     * This function evaluates whether a mesh has UV coordinates that span the full height (from 0 to 1),
     * which indicates that it requires flipping.
     * 
     * @param mesh The mesh to evaluate
     * @return True if the mesh requires texture flipping, false otherwise
     */
    bool meshRequireFlip(const aiMesh* mesh);

    /**
     * @brief Processes the nodes in the Assimp scene hierarchy
     * 
     * The function recursively processes all nodes in the scene. For each node, it processes the meshes 
     * associated with that node and adds them to the model.
     * 
     * @param node The Assimp node to process
     * @param scene The Assimp scene object
     */
    void processNode(aiNode *node, const aiScene *scene);

    /**
     * @brief Processes a single mesh and converts it into a Mesh object
     * 
     * The function processes the mesh by extracting vertex positions, normals, texture coordinates, 
     * and indices. It also handles applying materials and textures to the mesh.
     * 
     * @param mesh The Assimp mesh to process
     * @param scene The Assimp scene object
     * @return A unique pointer to the processed Mesh object
     */
    std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);

    /**
     * @brief Loads the textures associated with a material
     * 
     * This function loads textures of a given type from the material of a mesh. It avoids reloading 
     * duplicate textures by checking a list of already loaded textures.
     * 
     * @param mat The Assimp material object
     * @param type The type of texture to load (e.g., diffuse, specular)
     * @param typeName The name of the texture type (e.g., "texture_diffuse")
     * @return A vector of loaded Texture objects
     */
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    /**
     * @brief Creates a texture from a file
     * 
     * This function loads a texture image from disk and generates an OpenGL texture ID. It supports 
     * various texture formats and generates mipmaps for efficient rendering.
     * 
     * @param path The path to the texture file
     * @param directory The directory where the texture is located
     * @return The generated OpenGL texture ID
     */
    unsigned int textureFromFile(const std::string &path, const std::string &directory);

    /**
     * @brief Applies a default white texture when no texture is found
     * 
     * This function generates a simple 1x1 white texture and returns its OpenGL texture ID.
     * 
     * @return The OpenGL texture ID for the default white texture
     */
    unsigned int applyNullTexture();

public:
    /**
     * @brief Constructs a new Model object from a file
     * 
     * The constructor loads a 3D model from a file, processes the meshes, and calculates the model's 
     * dimensions (size, center, radius).
     * 
     * @param path The path to the model file
     */
    Model(const std::string &path);

    /**
     * @brief Destructor to clean up resources
     * 
     * The destructor cleans up the loaded textures and meshes, releasing any resources used by the model.
     */
    ~Model();

    /**
     * @brief Cleans up all resources associated with the model
     * 
     * The function deletes the OpenGL textures and clears the loaded textures and meshes.
     */
    void cleanup();

    /**
     * @brief Calculates the dimensions of the model
     * 
     * The function calculates the size, center, and radius of the model's bounding box.
     */
    void calculateModelDimension();

    /**
     * @brief Draws the model using a given shader program
     * 
     * This function iterates through all meshes in the model and renders them using the provided shader program.
     * 
     * @param shader The shader program to use for rendering
     */
    void draw(ShaderProgram &shader);

    /**
     * @brief Gets the center of the model's bounding box
     * 
     * @return The model's center as a glm::vec3
     */
    const glm::vec3 getModelCenter() const;

    /**
     * @brief Gets the radius of the model's bounding sphere
     * 
     * @return The model's radius
     */
    const float getModelRadius() const;
};
