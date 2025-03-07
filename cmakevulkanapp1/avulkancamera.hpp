#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm> // For glm::clamp

namespace Camera {

    enum class Direction {
        Forward = 0,
        Backward,
        Left,
        Right
    };

    struct State {
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        float yaw;
        float pitch;
        float speed;
        float sensitivity;
    };

    auto updateVectors(State& cam) -> void {
        // Calculate the front vector based on yaw and pitch
        glm::vec3 front;
        front.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
        front.y = sin(glm::radians(cam.pitch));
        front.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
        cam.front = glm::normalize(front);

        // Right vector is perpendicular to the front and worldUp vectors
        cam.right = glm::normalize(glm::cross(cam.front, cam.worldUp));

        // Up vector is perpendicular to both the right and front vectors
        cam.up = glm::normalize(glm::cross(cam.right, cam.front));
    }

    auto create(glm::vec3 pos, glm::vec3 up, float yaw, float pitch) -> State {
        State cam{ pos, glm::vec3(0.0f, 0.0f, -1.0f), up, {}, up, yaw, pitch, 2.5f, 0.1f };
        updateVectors(cam); // Initialize the direction vectors
        return cam;
    }

    auto getViewMatrix(const State& cam) -> glm::mat4 {
        // Create and return the view matrix using camera position, front, and up
        return glm::lookAt(cam.position, cam.position + cam.front, cam.up);
    }

    constexpr auto processKeyboard(State& cam, Direction direction, float deltaTime) -> void {
        float velocity = cam.speed * deltaTime;

        switch (direction) {
        case Direction::Forward:
            cam.position += cam.front * velocity;
            break;
        case Direction::Backward:
            cam.position -= cam.front * velocity;
            break;
        case Direction::Left:
            cam.position -= cam.right * velocity;
            break;
        case Direction::Right:
            cam.position += cam.right * velocity;
            break;
        }
    }

    auto processMouse(State& cam, float xOffset, float yOffset) -> void {
        // Update yaw and pitch based on mouse input
        xOffset *= cam.sensitivity;
        yOffset *= cam.sensitivity;

        cam.yaw += xOffset;
        cam.pitch -= yOffset;

        // Constrain pitch to prevent flipping
        cam.pitch = glm::clamp(cam.pitch, -89.0f, 89.0f);

        // Recalculate the front, right, and up vectors after pitch/yaw change
        updateVectors(cam);
    }

} // namespace Camera
