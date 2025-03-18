#pragma once

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <iostream>
#include "constants.hpp"

class Camera {
private:
    glm::vec3 modelCenter;

    float modelRadius;

    bool cameraRotationEnabled = true;
    bool freeCameraEnabled = false;

    // Global Y axis
    glm::vec3 globalUp;

    glm::vec3 cameraFront;

    // World space vectort pointing to camera position
    glm::vec3 cameraPos;

    // Camera target set as Origin
    glm::vec3 cameraTarget;

    // Vector that points to the Z axis
    glm::vec3 cameraDirection;

    // Vector that points to the X axis
    glm::vec3 cameraRight;

    // Camera Y axis Vector
    glm::vec3 cameraUp;

    float cameraSpeed;

    // Yaw angle: Rotates the camera left/right around the Y-axis (horizontal rotation).
    float yaw = DEFAULT_YAW_ANGLE;

    // Pitch angle: Rotates the camera up/down around the X-axis (vertical rotation).
    float pitch = DEFAULT_PITCH_ANGLE;

    // Standard FOV
    float fov = DEFAULT_CAMERA_FOV;

    void applyMovementSmoothing(glm::vec3 targetPos);

    void moveForward();

    void moveBackward();

    void moveRight();

    void moveLeft();

    void init();

public:
    Camera();
    ~Camera() = default;

    Camera(float modelRadius, glm::vec3 modelCenter);

    void applyZoom(float yOffset);

    void applyRotation(float xOffset, float yOffset);

    void setCameraPos(glm::vec3 newCameraPos);

    void updateCameraSpeed(float deltaTime);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    const glm::vec3 getCameraPos() const;

    const glm::vec3 getCameraFront() const;

    const glm::vec3 getGlobalUp() const;

    const float getCameraSpeed() const;

    const float getFov() const;

    void move(const std::string& dir);

    bool* getIsCameraRotationEnabled() { return &this->cameraRotationEnabled; }

    bool* getIsFreeCameraEnabled() { return &this->freeCameraEnabled; }

    void calculateYawPitchFromVector(const glm::vec3& direction);

    void reset();
};
