#include "mvp.hpp"
#include "constants.hpp"
#include <SDL2/SDL_timer.h>

Mvp::Mvp() {
    model = IDENTITY_MATRIX;
    view = IDENTITY_MATRIX;
    projection = IDENTITY_MATRIX;
}

void Mvp::updateMatrices(Camera& camera) {

    // TODO: Make this controlled by UI
    if (MODEL_ROTATION_MODE == "Natural Rotation") {
        updateModel();
    } else {
        updateModel(camera);
    }

    updateView(camera);
    updateProjection(camera);
}

void Mvp::updateModel() {
    model = glm::rotate(IDENTITY_MATRIX, (float)SDL_GetTicks64() / 1000.0f, DEFAULT_ROTATION_AXIS);
}

void Mvp::updateModel(Camera& camera) {
    model = glm::rotate(IDENTITY_MATRIX, glm::radians(modelPitch), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(modelYaw), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Mvp::updateView(Camera& camera) {
    view = camera.getLookAtMatrix();
}

void Mvp::updateProjection(Camera& camera) {
    projection = glm::perspective(glm::radians(camera.getFov()), DEFAULT_ASPECT_RATIO, DEFAULT_NEAR_CLIPPING_PLANE, DEFAULT_FAR_CLIPPING_PLANE);
}

void Mvp::updateModelYaw(float xoffset) {
    modelYaw += xoffset;
}

void Mvp::updateModelPitch(float yoffset) {
    modelPitch += yoffset;
}

const glm::mat4 Mvp::getModel() const {
    return model;
}

const glm::mat4 Mvp::getView() const {
    return view;
}

const glm::mat4 Mvp::getProjection() const {
    return projection;
}

