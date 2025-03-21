#pragma once

#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <iostream>
#include "utils/constants.hpp"

/**
 * @brief Handles camera operations such as movement, rotation, and zooming for 3D scenes.
 */
class Camera {
private:
    glm::vec3 modelCenter; ///< The center of the model that the camera is focused on.
    float modelRadius; ///< The radius of the model from the camera.

    bool cameraRotationEnabled = true; ///< Flag to enable or disable camera rotation.
    bool freeCameraEnabled = false; ///< Flag to enable or disable free camera mode (manual movement).
    
    glm::vec3 globalUp; ///< The global up direction (Y-axis).
    glm::vec3 cameraFront; ///< The camera's front vector (direction it's looking).
    glm::vec3 cameraPos; ///< The position of the camera in world space.
    glm::vec3 cameraTarget; ///< The target the camera is looking at (usually set to the origin).
    glm::vec3 cameraDirection; ///< The direction vector pointing to the camera's Z-axis.
    glm::vec3 cameraRight; ///< The direction vector pointing to the camera's X-axis (right).
    glm::vec3 cameraUp; ///< The direction vector pointing to the camera's Y-axis (up).

    float cameraSpeed; ///< The speed at which the camera moves.

    float yaw = DEFAULT_YAW_ANGLE; ///< Yaw angle for rotating the camera around the Y-axis (left/right).
    float pitch = DEFAULT_PITCH_ANGLE; ///< Pitch angle for rotating the camera around the X-axis (up/down).
    float fov = DEFAULT_CAMERA_FOV; ///< Field of view for the camera (vertical angle of the camera's view).

    /**
     * @brief Applies smoothing to the camera's movement towards a target position.
     * 
     * @param targetPos The target position to smooth towards.
     */
    void applyMovementSmoothing(glm::vec3 targetPos);

    /** 
     * @brief Moves the camera forward based on its current direction.
     */
    void moveForward();

    /** 
     * @brief Moves the camera backward based on its current direction.
     */
    void moveBackward();

    /** 
     * @brief Moves the camera to the right (perpendicular to its front direction).
     */
    void moveRight();

    /** 
     * @brief Moves the camera to the left (perpendicular to its front direction).
     */
    void moveLeft();

    /** 
     * @brief Initializes the camera's position and direction based on model parameters.
     */
    void init();

public:
    /** 
     * @brief Default constructor, initializes the camera to a default position and configuration.
     */
    Camera();

    /** 
     * @brief Destructor for the Camera class.
     */
    ~Camera() = default;

    /** 
     * @brief Constructor that sets the camera configuration based on model radius and center.
     * 
     * @param modelRadius The radius of the model to be displayed.
     * @param modelCenter The center of the model to focus the camera on.
     */
    Camera(float modelRadius, glm::vec3 modelCenter);

    /** 
     * @brief Configures the camera position and orientation based on the model's radius and center.
     */
    void setCameraConfiguration();

    /** 
     * @brief Adjusts the camera's field of view (zoom) based on mouse scroll input.
     * 
     * @param yOffset The amount of zoom applied (positive for zooming out, negative for zooming in).
     */
    void applyZoom(float yOffset);

    /** 
     * @brief Rotates the camera based on mouse input (X and Y offsets).
     * 
     * @param xOffset The amount of rotation around the Y-axis.
     * @param yOffset The amount of rotation around the X-axis.
     */
    void applyRotation(float xOffset, float yOffset);

    /** 
     * @brief Sets the camera's position.
     * 
     * @param newCameraPos The new position of the camera.
     */
    void setCameraPos(glm::vec3 newCameraPos);

    /** 
     * @brief Updates the camera's movement speed based on time passed.
     * 
     * @param deltaTime The time difference between frames.
     */
    void updateCameraSpeed(float deltaTime);

    /** 
     * @brief Returns the camera's view matrix for rendering.
     * 
     * @return The view matrix for the camera.
     */
    glm::mat4 getViewMatrix() const;

    /** 
     * @brief Returns the camera's projection matrix for rendering.
     * 
     * @return The projection matrix for the camera.
     */
    glm::mat4 getProjectionMatrix() const;

    /** 
     * @brief Returns the camera's position in world space.
     * 
     * @return The current position of the camera.
     */
    const glm::vec3 getCameraPos() const;

    /** 
     * @brief Returns the direction the camera is looking at.
     * 
     * @return The camera's front direction vector.
     */
    const glm::vec3 getCameraFront() const;

    /** 
     * @brief Returns the global up direction vector (usually the Y-axis).
     * 
     * @return The global up direction vector.
     */
    const glm::vec3 getGlobalUp() const;

    /** 
     * @brief Returns the camera's movement speed.
     * 
     * @return The current speed of the camera.
     */
    const float getCameraSpeed() const;

    /** 
     * @brief Returns the camera's current field of view (FOV).
     * 
     * @return The current field of view.
     */
    const float getFov() const;

    /** 
     * @brief Moves the camera in a specified direction.
     * 
     * @param dir The direction to move in ("w", "s", "a", or "d").
     */
    void move(const std::string& dir);

    /** 
     * @brief Returns a pointer to the camera rotation enable flag.
     * 
     * @return A pointer to the flag controlling camera rotation.
     */
    bool* getIsCameraRotationEnabled() { return &this->cameraRotationEnabled; }

    /** 
     * @brief Returns a pointer to the free camera mode enable flag.
     * 
     * @return A pointer to the flag controlling free camera mode.
     */
    bool* getIsFreeCameraEnabled() { return &this->freeCameraEnabled; }

    /** 
     * @brief Calculates the yaw and pitch angles from a direction vector.
     * 
     * @param direction The direction vector to calculate yaw and pitch from.
     */
    void calculateYawPitchFromVector(const glm::vec3& direction);

    /** 
     * @brief Resets the camera to its default configuration.
     */
    void reset();
};
