#pragma once

#include <glm/glm.hpp>

// TODO: Make this dependend on UI selection
// Option: 1. Input Rotation 2. Natural Rotation
#define MODEL_ROTATION_MODE "Input Rotation"

#define DEFAULT_CAMERA_SPEED 0.05f
#define DEFAULT_CAMERA_FOV 45.0f
#define DEFAULT_CAMERA_SENSITIVITY 0.05f

#define DEFAULT_MODEL_ROTATION_SENSITIVITY 0.1f

// A default value of -90.0f yaw angle ensures the camera initially faces forward along the negative Z-axis.
#define DEFAULT_YAW_ANGLE -90.0f

// A default value of 0.0f keeps the camera level, preventing an initial tilt.
#define DEFAULT_PITCH_ANGLE 0.0f
#define MAX_CAMERA_PITCH_ANGLE 89.0f
#define MIN_CAMERA_PITCH_ANGLE -89.0f

#define SPINNING_CAMERA_RADIUS 10.0f

// Define visible depth range
#define DEFAULT_NEAR_CLIPPING_PLANE 0.1f
#define DEFAULT_FAR_CLIPPING_PLANE 100.0f


// Define light constants
#define DEFAULT_LIGHT_AMBIENT 0.1f
#define DEFAULT_LIGHT_DIFFUSE 0.5f
#define DEFAULT_LIGHT_SPECULAR 1.0f

// Define light attenuation constants
#define DEFAULT_LIGHT_CONSTANT 1.0f
#define DEFAULT_LIGHT_LINEAR 0.09f
#define DEFAULT_LIGHT_QUADRATIC 0.032f

// Define light colour
#define DEFAULT_LIGHT_COLOUR glm::vec3(1.0f)

// Define light numbers
#define MAX_POINT_LIGHTS 10
#define MAX_DIRECTIONAL_LIGHTS 3

// Width / Height to prevent distortion
constexpr float DEFAULT_ASPECT_RATIO = 800.0f / 600.0f;

const glm::mat4 IDENTITY_MATRIX = glm::mat4(1.0f);
const glm::vec3 DEFAULT_ROTATION_AXIS = glm::vec3(0.5f, 1.0f, 0.0f);
const glm::vec3 DEFAULT_GLOBAL_UP = glm::vec3(0.0f, 1.0f, 0.0f);
