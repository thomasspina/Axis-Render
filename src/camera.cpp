#include "camera.hpp"
#include <SDL2/SDL_timer.h>

Camera::Camera() {
    globalUp = DEFAULT_GLOBAL_UP;
    cameraFront = glm::vec3(0.0f, -0.8f, -0.2f);
    cameraPos = glm::vec3(0.0f, 2.0f, 0.0f);
    cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    cameraRight = glm::normalize(glm::cross(globalUp, cameraDirection));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));
}

Camera::Camera(float modelRadius, glm::vec3 modelCenter) {
    globalUp = DEFAULT_GLOBAL_UP;
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);


    // Calculates camera distance required so the model sphere fits within the camera's vertical fov
    float requiredDistance = modelRadius / sin(glm::radians(fov) / 2.0f);

    this->modelRadius = requiredDistance;
    this->modelCenter = modelCenter;

    cameraPos = modelCenter + glm::vec3(0.0f, 0.0f, requiredDistance);
    cameraTarget = modelCenter;

    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    cameraRight = glm::normalize(glm::cross(globalUp, cameraDirection));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));
}

void Camera::applyZoom(float yOffset) {
    fov -= yOffset;

    if (fov < 1.0f) {
        fov = 1.0f;
    }

    if (fov > 45.0f) {
        fov = 45.0f;
    }
}

void Camera::applyRotation(float xOffset, float yOffset) {
    if (cameraRotationEnabled) {
        xOffset *= DEFAULT_CAMERA_SENSITIVITY;
        yOffset *= DEFAULT_CAMERA_SENSITIVITY;

        yaw += xOffset;
        pitch += yOffset;

        // Clamp pitch to avoid lookat flipping
        if (pitch > MAX_CAMERA_PITCH_ANGLE)
            pitch = MAX_CAMERA_PITCH_ANGLE;
        if (pitch < MIN_CAMERA_PITCH_ANGLE)
            pitch = MIN_CAMERA_PITCH_ANGLE;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }
}

void Camera::setCameraPos(glm::vec3 newCameraPos) {
    cameraPos = newCameraPos;
}

void Camera::updateCameraSpeed(float deltaTime) {
    cameraSpeed = DEFAULT_CAMERA_SPEED * deltaTime;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, globalUp);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), DEFAULT_ASPECT_RATIO, DEFAULT_NEAR_CLIPPING_PLANE, DEFAULT_FAR_CLIPPING_PLANE);
}

const glm::vec3 Camera::getCameraPos() const {
    return cameraPos;
}

const glm::vec3 Camera::getCameraFront() const {
    return cameraFront;
}

const glm::vec3 Camera::getGlobalUp() const {
    return globalUp;
}

const float Camera::getCameraSpeed() const {
    return cameraSpeed;
}

const float Camera::getFov() const {
    return fov;
}

void Camera::move(const std::string& dir) {
    if (freeCameraEnabled) {
        if (dir == "w") {
            moveForward();
        } else if (dir == "s") {
            moveBackward();
        } else if (dir == "a") {
            moveLeft();
        } else if (dir == "d") {
            moveRight();
        }
    }
}


void Camera::moveForward() {
    cameraPos += cameraSpeed * cameraFront;
}

void Camera::moveBackward() {
    cameraPos -= cameraSpeed * cameraFront;
}

void Camera::moveRight() {
    cameraPos += glm::normalize(glm::cross(cameraFront, globalUp)) * cameraSpeed;
}

void Camera::moveLeft() {
    cameraPos -= glm::normalize(glm::cross(cameraFront, globalUp)) * cameraSpeed;
}

