#pragma once
#include <glm/glm.hpp>
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

        void applyZoom(float yOffset);

        void applyRotation(float xOffset, float yOffset);

        void setCameraPos(glm::vec3 newCameraPos);

        void updateCameraSpeed(float deltaTime);

        const glm::vec3 getCameraPos();

        const glm::vec3 getCameraFront();

        const glm::vec3 getGlobalUp();

        const float getCameraSpeed();

        const float getFov();
};
