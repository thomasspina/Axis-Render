#pragma once

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "constants.hpp"

class Camera {
    private:
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

    public:
        Camera();

        Camera(float modelRadius, glm::vec3 modelCenter);

        void applyZoom(float yOffset);

        void applyRotation(float xOffset, float yOffset);

        void setCameraPos(glm::vec3 newCameraPos);

        void updateCameraSpeed(float deltaTime);

        const glm::mat4 getViewMatrix() const;

        const glm::mat4 getProjectionMatrix() const;

        const glm::vec3 getCameraPos() const;

        const glm::vec3 getCameraFront() const;

        const glm::vec3 getGlobalUp() const;

        const float getCameraSpeed() const;

        const float getFov() const;

        void moveForward();

        void moveBackward();

        void moveRight();

        void moveLeft();
};
