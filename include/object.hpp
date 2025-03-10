#pragma once

#include "constants.hpp"
#include <glm/glm.hpp>

class Object {
private:
    float objectYaw;
    float objectPitch;
    int rotationMode = Rotation::naturalRotationMode;
    
protected:
    glm::vec3 position = glm::vec3(0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
    
public:
    Object();
    Object(glm::vec3 position);
    virtual ~Object();

    const glm::mat4& getModelMatrix() const;
    const glm::mat3& getNormalMatrix() const;
    const glm::vec3& getPosition() const;

    void setPosition(const glm::vec3& newPosition);

    void translate(const glm::vec3& translation);
    void rotate(float angle, const glm::vec3& axis);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void scale(const glm::vec3& scale);
    void scale(float scale);

    void resetModel();

    void updateNormalMatrix(const glm::mat4& view);

    void updateObjectYaw(float yoffset);
    void updateObjectPitch(float xoffset);

    void updateModelMatrix();
    void naturalRotation();
    void inputRotation();
    
    void setRotationMode(int newRotationMode) { rotationMode = newRotationMode; }
    int getRotationalMode() { return this->rotationMode; }
};
