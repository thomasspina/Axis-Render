#pragma once

#include <glm/glm.hpp>

namespace CameraMode {
    constexpr int freeCamera = 0;
    constexpr int orbitCamera = 1;
}

enum class RotationMode {
    INPUT_ROTATION = 0,
    NATURAL_ROTATION
};

namespace ModelSelection {
    constexpr const char* models[] = { 
        "Backpack",
        "Brick Cylinder",
        "Bunny",
        "Cube",
        "Cyborg",
        "Nanosuit",
        "Planet",
        "Space Shuttle",
        "Vase",
        "Head",
        "Triceratops"
    };

}

namespace ShaderSelection {
    constexpr const char* shaders[] = { 
        "Phong",
        "Gouraud",
        "Grayscale",
        "Sketch",
        "ASCII", 
    };
}

#define AVG_MEMORY_USAGE 50.0f
#define SCREEN_FPS 60u
#define OPEN_GL_VERSION "#version 460"

// TODO: Make this dependend on UI selection
// Option: 1. Input Rotation 2. Natural Rotation
#define MODEL_ROTATION_MODE "Input Rotation"
// #define MODEL_ROTATION_MODE "Natural Rotation"


/****************************************/
/*           Camera Constants           */
/****************************************/

#define DEFAULT_CAMERA_SPEED_SCALING_FACTOR 200
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


/****************************************/
/*          Lighting Constants          */
/****************************************/

// Define visible depth range
#define DEFAULT_NEAR_CLIPPING_PLANE 0.1f
#define DEFAULT_FAR_CLIPPING_PLANE 500.0f

// Define light constants
#define DEFAULT_LIGHT_AMBIENT 0.3f
#define DEFAULT_LIGHT_DIFFUSE 0.7f
#define DEFAULT_LIGHT_SPECULAR 0.5f

// Define light attenuation constants
#define DEFAULT_LIGHT_CONSTANT 1.0f
#define DEFAULT_LIGHT_LINEAR 0.09f
#define DEFAULT_LIGHT_QUADRATIC 0.032f

// Define light colour
#define DEFAULT_LIGHT_COLOUR glm::vec3(1.0f)

// Define light numbers
#define MAX_POINT_LIGHTS 4

// Define light intensity
#define DEFAULT_LIGHT_INTENSITY 1.0f

// Define light caster direction
#define DEFAULT_LIGHT_CASTER_DIRECTION glm::vec3(-0.2f, -1.0f, -0.3f)

// Define point light mesh size
#define DEFAULT_POINT_LIGHT_SIZE 0.5f


/****************************************/
/*           Other Constants            */
/****************************************/

#define DEFAULT_SHININESS 30.0f

// Width / Height to prevent distortion
constexpr float DEFAULT_ASPECT_RATIO = 800.0f / 600.0f;

const glm::mat4 IDENTITY_MATRIX = glm::mat4(1.0f);
const glm::vec3 DEFAULT_ROTATION_AXIS = glm::vec3(0.5f, 1.0f, 0.5f);
const glm::vec3 DEFAULT_GLOBAL_UP = glm::vec3(0.0f, 1.0f, 0.0f);
