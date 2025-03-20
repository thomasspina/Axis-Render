#include <glm/gtc/matrix_transform.hpp>
#include "object.hpp"

Object::Object(glm::vec3 position) : position(position) {
    updateModelMatrix();
}

void Object::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
    updateModelMatrix();
}

void Object::translate(const glm::vec3& translation) {
    position += translation;
    updateModelMatrix();
}

void Object::rotate(float angle, const glm::vec3& axis) {
    glm::vec3 normalizedAxis = glm::normalize(axis);

    pitch += angle * normalizedAxis.x;
    yaw += angle * normalizedAxis.y;
    roll += angle * normalizedAxis.z;

    updateModelMatrix();
}

void Object::scale(float scale) {
    objScale = scale;
    updateModelMatrix();
}

void Object::resetModel() {
    yaw = 0.0f;
    pitch = 0.0f;
    roll = 0.0f;
    objScale = 1.0f;

    model = IDENTITY_MATRIX;
    updateModelMatrix();
}

void Object::updateNormalMatrix(const glm::mat4& view) {
    normalMatrix = glm::transpose(glm::inverse(view * model));
}

void Object::incrementYaw(float incr) {
    yaw += incr;
    updateModelMatrix();
}

void Object::incrementPitch(float incr) {
    pitch += incr;
    updateModelMatrix();
}

void Object::incrementRoll(float incr) {
    roll += incr;
    updateModelMatrix();
}

void Object::updateModelMatrix() {
    model = IDENTITY_MATRIX;
        
    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));   // Y-axis
    model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
    model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));  // Z-axis

    model = glm::scale(model, glm::vec3(objScale));
}
