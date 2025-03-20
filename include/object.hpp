#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL_timer.h>

#include "utils/constants.hpp"

class Object {
private:
    float yaw = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;

    float objScale = 1.0f;

    void updateModelMatrix();
protected:
    glm::vec3 position = glm::vec3(0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
    
public:
    Object() = default;
    Object(glm::vec3 position);
    virtual ~Object() = default;

    glm::mat4 getModelMatrix() const { return model; }
    glm::mat3 getNormalMatrix() const { return normalMatrix;}
    glm::vec3 getPosition() const { return position; }
    float getModelScale() const { return objScale; }

    void setPosition(const glm::vec3& newPosition);

    void rotate(float angle, const glm::vec3& axis);
    void translate(const glm::vec3& translation);
    void scale(float scale);

    void resetModel();

    void updateNormalMatrix(const glm::mat4& view);

    void incrementYaw(float incr);
    void incrementPitch(float incr);
    void incrementRoll(float incr);
};
