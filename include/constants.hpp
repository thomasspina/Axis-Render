#pragma once

#define DEFAULT_CAMERA_SPEED 0.05f
#define DEFAULT_CAMERA_FOV 45.0f
#define DEFAULT_CAMERA_SENSITIVITY 0.1f

// A default value of -90.0f yaw angle ensures the camera initially faces forward along the negative Z-axis.
#define DEFAULT_YAW_ANGLE -90.0f

// A default value of 0.0f keeps the camera level, preventing an initial tilt.
#define DEFAULT_PITCH_ANGLE -90.0f
#define MAX_CAMERA_PITCH_ANGLE 89.0f
#define MIN_CAMERA_PITCH_ANGLE -89.0f

#define SPINNING_CAMERA_RADIUS 10.0f
