#ifndef INPUT_H
#define INPUT_H

#include "../VKconfiguration/kroPipe_buffer.hpp"
#include "../kroPipe_depedence.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_model.hpp"

void processInput(GLFWwindow *window, KP::OBJECT::Object& obj, float deltaTime, float baseSpeed = 3.0f, float sprintSpeed = 5.0f, float walkSpeed = 1.5f, float acceleration = 10.0f, float friction = 8.0f) {
    float targetSpeed = baseSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        targetSpeed = sprintSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        targetSpeed = walkSpeed;
    }

    glm::vec3 inputDir(0.0f);

    if (!obj.is_OnAir) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) inputDir.z += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) inputDir.z -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) inputDir.x -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) inputDir.x += 1.0f;

        if (glm::length(inputDir) > 0.0f) {
            inputDir = glm::normalize(inputDir);

            glm::vec3 forward = cameraPlayer.Front;
            forward.y = 0.0f;
            forward = glm::normalize(forward);
            glm::vec3 right = glm::normalize(glm::cross(forward, cameraPlayer.Up));

            glm::vec3 moveDir = inputDir.z * forward + inputDir.x * right;

            obj.velocity += moveDir * acceleration * deltaTime;
        } else {
            obj.velocity.x = glm::mix(obj.velocity.x, 0.0f, friction * deltaTime);
            obj.velocity.z = glm::mix(obj.velocity.z, 0.0f, friction * deltaTime);
        }
        glm::vec2 horizontalVel(obj.velocity.x, obj.velocity.z);
        float speed = glm::length(horizontalVel);
        if (speed > targetSpeed) {
            horizontalVel = glm::normalize(horizontalVel) * targetSpeed;
            obj.velocity.x = horizontalVel.x;
            obj.velocity.z = horizontalVel.y;
        }
    }

    obj.Position += glm::vec3(obj.velocity.x, 0.0f, obj.velocity.z) * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !obj.is_OnAir) {
        obj.velocity.y = 5.0f;
        obj.is_OnAir = true;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        obj.Position = glm::vec3(10.0f, 10.0f, 0.0f);
    }
}

#endif // INPUT_H
