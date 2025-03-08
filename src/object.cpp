#include <glm/gtc/matrix_transform.hpp>

#include "object.hpp"

Object::Object() = default;

Object::Object(glm::vec3 position) : position(position) {}

Object::~Object() = default;

const glm::mat4& Object::getModel() const {
    return model;
}

const glm::mat4& Object::getNormalMatrix() const {
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
    model = glm::mat4(1.0f);
}

void Object::updateNormalMatrix(const glm::mat4& view) {
    normalMatrix = glm::transpose(glm::inverse(view * model));
}