#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL_timer.h>
#include "utils/constants.hpp"

/**
 * @class Object
 * @brief Base class for 3D objects with transformation capabilities.
 * 
 * Provides functionality for positioning, rotating, and scaling objects in 3D space.
 * Maintains internal model and normal matrices for rendering and lighting calculations.
 * This class serves as a foundation for more specific object types in the engine.
 */
class Object {
private:
    /** @brief Rotation around Y-axis in degrees */
    float yaw = 0.0f;
    
    /** @brief Rotation around X-axis in degrees */
    float pitch = 0.0f;
    
    /** @brief Rotation around Z-axis in degrees */
    float roll = 0.0f;
    
    /** @brief Uniform scale factor for the object */
    float objScale = 1.0f;
    
    /**
     * @brief Updates the model matrix based on current position, rotation, and scale.
     * 
     * Reconstructs the model matrix by applying transformations in the order:
     * translation, rotation (yaw, pitch, roll), and scaling.
     */
    void updateModelMatrix();

protected:
    /** @brief Object's position in world space */
    glm::vec3 position = glm::vec3(0.0f);
    
    /** @brief Model transformation matrix */
    glm::mat4 model = glm::mat4(1.0f);
    
    /** @brief Normal transformation matrix for lighting calculations */
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

public:
    /** @brief Default constructor */
    Object() = default;
    
    /**
     * @brief Construct object at specified position
     * @param position Initial position in world space
     */
    Object(glm::vec3 position);
    
    /** @brief Virtual destructor for proper inheritance */
    virtual ~Object() = default;
    
    /**
     * @brief Get the object's model matrix
     * @return The current model transformation matrix
     */
    glm::mat4 getModelMatrix() const { return model; }
    
    /**
     * @brief Get the object's normal matrix for lighting calculations
     * @return The current normal transformation matrix
     */
    glm::mat3 getNormalMatrix() const { return normalMatrix; }
    
    /**
     * @brief Get the object's current position
     * @return The position vector in world space
     */
    glm::vec3 getPosition() const { return position; }
    
    /**
     * @brief Get the object's uniform scale factor
     * @return The current scale value
     */
    float getModelScale() const { return objScale; }
    
    /**
     * @brief Set the object's position
     * @param newPosition New position in world space
     */
    void setPosition(const glm::vec3& newPosition);
    
    /**
     * @brief Rotate the object around a specified axis
     * @param angle Rotation angle in degrees
     * @param axis Normalized axis vector to rotate around
     */
    void rotate(float angle, const glm::vec3& axis);
    
    /**
     * @brief Translate the object relative to its current position
     * @param translation The displacement vector to apply
     */
    void translate(const glm::vec3& translation);
    
    /**
     * @brief Set the object's uniform scale factor
     * @param scale New scale value (1.0 = original size)
     */
    void scale(float scale);
    
    /**
     * @brief Reset all transformations to default values
     * 
     * Sets position to (0,0,0), rotation angles to 0, scale to 1.0,
     * and resets the model matrix to identity.
     */
    void resetModel();
    
    /**
     * @brief Update the normal matrix based on the current model and view matrices
     * @param view The current view matrix
     * 
     * Computes the normal matrix as the transpose of the inverse of the view-model matrix.
     * This is used for transforming normals correctly when rendering with lighting.
     */
    void updateNormalMatrix(const glm::mat4& view);
    
    /**
     * @brief Increment the yaw angle (rotation around Y-axis)
     * @param incr Value in degrees to add to current yaw
     */
    void incrementYaw(float incr);
    
    /**
     * @brief Increment the pitch angle (rotation around X-axis)
     * @param incr Value in degrees to add to current pitch
     */
    void incrementPitch(float incr);
    
    /**
     * @brief Increment the roll angle (rotation around Z-axis)
     * @param incr Value in degrees to add to current roll
     */
    void incrementRoll(float incr);
};