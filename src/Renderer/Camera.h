#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.1f;
const float FOV = 60.0f;

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;

    float cameraYaw;
    float cameraPitch;

    float moveSpeed;
    float mouseSensitivity;
    float cameraFOV;

public:

    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : cameraPosition(pos), cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(up), cameraYaw(yaw), cameraPitch(pitch), moveSpeed(SPEED), mouseSensitivity(SENSITIVITY), cameraFOV(FOV)
    {
        updateCameraVec();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    }

    void processKeyboard(CameraMovement direction, float deltaTime) {
        float velocity = moveSpeed * deltaTime;
        if (direction == FORWARD)
            cameraPosition += cameraFront * velocity;
        if (direction == BACKWARD)
            cameraPosition -= cameraFront * velocity;
        if (direction == LEFT)
            cameraPosition -= cameraRight * velocity;
        if (direction == RIGHT)
            cameraPosition += cameraRight * velocity;
        if (direction == UP)
            cameraPosition += worldUp * velocity;
        if (direction == DOWN)
            cameraPosition -= worldUp * velocity;
    }

    void processMouseMovement(float xOffset, float yOffset, bool limitPitch = true) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        cameraYaw += xOffset;
        cameraPitch += yOffset;

        if (limitPitch) {
            if (cameraPitch > 89.9f)
                cameraPitch = 89.9f;
            if (cameraPitch < -89.9f)
                cameraPitch = -89.9f;
        }

        updateCameraVec();
    }

private:

    void updateCameraVec() {
        glm::vec3 front;
        front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        front.y = sin(glm::radians(cameraPitch));
        front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraFront = glm::normalize(front);

        cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
    }
};