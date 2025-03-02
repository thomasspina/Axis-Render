#include "model.hpp"

Model::Model(char *path) {
    loadModel(path);
}

void Model::draw(Shader &shader) {
    for(unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader);
    }
}
