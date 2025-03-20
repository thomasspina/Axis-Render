#include <iostream>
#include <stb_include/stb_image.h>
#include <cstring>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "rendering/model.hpp"
#include "utils/constants.hpp"


Model::Model(const std::string &path) {
    minBounds = glm::vec3(FLT_MAX);
    maxBounds = glm::vec3(-FLT_MAX);
    loadModel(path);
    calculateModelDimension();
}

Model::~Model() {
    cleanup();
}

void Model::cleanup() {

    for (const auto& texture: textures_loaded) {
        glDeleteTextures(1, &texture.id);
    }

    textures_loaded.clear();
    
    meshes.clear();
}

void Model::calculateModelDimension() {
    modelSize = maxBounds - minBounds;
    modelCenter = glm::vec3((maxBounds + minBounds) * 0.5f);
    modelRadius = glm::length(modelSize) * 0.5f;
}

void Model::draw(ShaderProgram &shader) {
    for (const auto& meshPtr : meshes) {
        meshPtr->draw(shader);
    }
}

const glm::vec3 Model::getModelCenter() const {
    return modelCenter;
}

const float Model::getModelRadius() const {
    return modelRadius;
}

void Model::loadModel(const std::string &path) {
    // Initially load without aiProcess_FlipUVs
    scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    bool globalFlip = shouldFlipModel(scene);

    // Check whether model does not require UV flip
    if (!globalFlip) {

        scene = importWithoutFlip.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << importWithoutFlip.GetErrorString() << std::endl;
            return;
        }
    }
    
    // Save model directory
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

bool Model::shouldFlipModel(const aiScene* scene) {
    int flipCount = 0;
    int totalCount = 0;
    
    for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
        aiMesh* mesh = scene->mMeshes[m];

        if (!mesh->HasTextureCoords(0)) continue;

        totalCount++;
        if (meshRequireFlip(mesh)) {
            flipCount++;
        }
    }
    
    if (totalCount == 0) return false; // no UVs in any mesh

    // If more than half requires flipping, whole model likely requires flipping
    return (flipCount > totalCount / 2);
}

bool Model::meshRequireFlip(const aiMesh* mesh) {
    float minV = 1.0f;
    float maxV = 0.0f;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        float v = mesh->mTextureCoords[0][i].y; // Access the y coordinate
        minV = std::min(minV, v);
        maxV = std::max(maxV, v);
    }

    // Indicate full height UV map used therefore needs flipping
    return (maxV > 0.9f && minV < 0.1f);
}

// aiScene is the top level object containing all the model info eg: meshes, textures
// aiNode is a single node within the scene, does not store mesh directly but holds indice to mesh within aiScene
void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all the node’s meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

// MMesh: Positions, Normals and texture coordinate
std::unique_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    float shininess = DEFAULT_SHININESS;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // Vertex positions
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // Get model's largest and smallest vector position values
        minBounds = glm::min(minBounds, vector);
        maxBounds = glm::max(maxBounds, vector);

        // Vertex normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        // Vertex texture coordinates
        // Check if primary coordinate map exist
        if(mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices on each (in our case) triangle face of the model
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // mMaterial: Textures, colors and shading settings
    // If mMaterialIndex is -1, then mesh contains no material
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        // Read map_Kd value
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Read map_Ks value
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        // Read map_Bump
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        // Read map_AO
        std::vector<Texture> aoMaps = loadMaterialTextures(material, aiTextureType_LIGHTMAP, "texture_ao");
        textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());

        // Read map_Pr
        std::vector<Texture> roughnessMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "texture_roughness");
        textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

        // Read map_Pm
        std::vector<Texture> metallicMaps = loadMaterialTextures(material, aiTextureType_REFLECTION, "texture_metallic");
        textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());

        // Apply shine if specular material is present
        if (!specularMaps.empty()) {
            float shininess_value;

            if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess_value)) {
                shininess = shininess_value;
            }
        }
    }

    // If mesh contains no texture, render a default null texture
    if (textures.empty()) {
        Texture defaultTexture;
        defaultTexture.id = applyNullTexture(); 
        defaultTexture.type = "texture_diffuse";
        textures.push_back(defaultTexture);
    }

    return std::make_unique<Mesh>(vertices, indices, textures, shininess);
}

unsigned int Model::applyNullTexture() {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // White color
    unsigned char whitePixel[3] = { 255, 255, 255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, whitePixel);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return textureID;
}

// Load all textures found within the given material
std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    // Iterate over all textures of given type
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        
        // Store texture path within str
        mat->GetTexture(type, i, &str);
        bool skip = false;

        // Iterate over already loaded textures to prevent duplicate loading
        for(unsigned int j = 0; j < textures_loaded.size(); j++) {

            // Texture has already been processed
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        
        // Texture hasn’t been loaded already, load it into the global textures vector
        if(!skip) { 
            Texture texture;
            texture.id = textureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); 
        }
    }

    return textures;
}

unsigned int Model::textureFromFile(const std::string &path, const std::string &directory){
    // texture file name
    std::string filename = directory + '/' + path;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    // nrComponents inform us of the number of color components within the given image
    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        // Grayscale
        if (nrComponents == 1)
            format = GL_RED;
        // Standard Color
        else if (nrComponents == 3)
            format = GL_RGB;
        // Include transparency
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);

        // Creates texture and sends it to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
