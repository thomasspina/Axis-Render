#include <glm/gtc/matrix_transform.hpp>
#include "object.hpp"

Object::Object() = default;

Object::Object(glm::vec3 position) : position(position) {
    model = glm::translate(model, position);
}

const glm::mat4& Object::getModelMatrix() const {
    return model;
}

const glm::mat3& Object::getNormalMatrix() const {
    return normalMatrix;
}

const glm::vec3& Object::getPosition() const {
    return position;
}

void Object::setPosition(const glm::vec3& newPosition) {
    glm::vec3 translation = newPosition - this->position;
    
    this->position = newPosition;
    
    model = glm::translate(model, translation);
}

void Object::translate(const glm::vec3& translation) {
    position += translation;

    model = glm::translate(model, translation);
}

void Object::rotate(float angle, const glm::vec3& axis) {
    model = glm::rotate(model, angle, axis);
}

void Object::rotateX(float angle) {
    model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Object::rotateY(float angle) {
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Object::rotateZ(float angle) {
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Object::scale(const glm::vec3& scale) {
    model = glm::scale(model, scale);
}

void Object::scale(float scale) {
    model = glm::scale(model, glm::vec3(scale));
}

void Object::resetModel() {
    objectYaw = 0.0f;
    objectPitch = 0.0f;
    model = IDENTITY_MATRIX;
    objectScale = 1.0f;

    initialTime = SDL_GetTicks64();
}

void Object::updateNormalMatrix(const glm::mat4& view) {
    normalMatrix = glm::transpose(glm::inverse(view * model));
}

void Object::updateObjectYaw(float xoffset) {
    if (rotationMode == RotationMode::inputRotation) {
        objectYaw += xoffset;
    }
}

void Object::updateObjectPitch(float yoffset) {
    if (rotationMode == RotationMode::inputRotation) {
        objectPitch += yoffset;

        // Prevent gimbal lock
        objectPitch = glm::clamp(objectPitch, -89.0f, 89.0f);
    }
}

void Object::updateModelMatrix() {
    model = IDENTITY_MATRIX;

    scale(objectScale);

    if (rotationMode == RotationMode::naturalRotation) {
        naturalRotation();
    } else if (rotationMode == RotationMode::inputRotation) {
        inputRotation();
    }
}

void Object::naturalRotation() {
    float elapsedTime = (float)((SDL_GetTicks64() - initialTime) / 1000.0f);
    model = glm::rotate(model, elapsedTime, DEFAULT_ROTATION_AXIS);
}

void Object::inputRotation() {
    model = glm::rotate(model, glm::radians(objectPitch), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(objectYaw), glm::vec3(0.0f, 1.0f, 0.0f));
}
