#ifndef INPUT_H
#define INPUT_H
#include "kroPipe_object.hpp"
#include "../VKconfiguration/kroPipe_buffer.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_model.hpp"

inline void playerWalk(GLFWwindow *window, KP::OBJECT::Object& object, float deltaTime, float baseSpeed = 3.0f, float sprintSpeed = 5.0f, float walkSpeed = 1.5f, float acceleration = 10.0f, float friction = 8.0f){

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        baseSpeed = sprintSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        baseSpeed = walkSpeed;
    }

    glm::vec3 inputDir(0.0f);

    if (!object.data.is_OnAir) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) inputDir.z += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) inputDir.z -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) inputDir.x -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) inputDir.x += 1.0f;

        if (glm::length(inputDir) > 0.0f) {
            inputDir = glm::normalize(inputDir);

            glm::vec3 forward = cameraPlayer.Front;
            //forward.y = 0.0f;
            forward = glm::normalize(forward);
            glm::vec3 right = glm::normalize(glm::cross(forward, cameraPlayer.Up));

            glm::vec3 moveDir = inputDir.z * forward + inputDir.x * right;

            object.data.velocity += moveDir * acceleration * deltaTime;
        } else {
            object.data.velocity.x = glm::mix(object.data.velocity.x, 0.0f, friction * deltaTime);
            object.data.velocity.z = glm::mix(object.data.velocity.z, 0.0f, friction * deltaTime);
        }
        glm::vec2 horizontalVel(object.data.velocity.x, object.data.velocity.z);
        float speed = glm::length(horizontalVel);
        if (speed > baseSpeed) {
            horizontalVel = glm::normalize(horizontalVel) * baseSpeed;
            object.data.velocity.x = horizontalVel.x;
            object.data.velocity.z = horizontalVel.y;
        }
    }

    object.data.Position += glm::vec3(object.data.velocity.x, 0.0f, object.data.velocity.z) * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !object.data.is_OnAir) {
        object.data.velocity.y = 5.0f;
        object.data.is_OnAir = true;
    }

}

inline void playerFly(GLFWwindow *window, KP::OBJECT::Object& object, float deltaTime, float baseSpeed = 0.01f, float sprintSpeed = 0.03f, float walkSpeed = 0.002f){
    
    
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        baseSpeed = sprintSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        baseSpeed = walkSpeed;
    }

    
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        baseSpeed = sprintSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        baseSpeed = walkSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    baseSpeed = sprintSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    baseSpeed = walkSpeed;


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPlayer.Position += baseSpeed * cameraPlayer.Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPlayer.Position -= baseSpeed * cameraPlayer.Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPlayer.Position -= glm::normalize(glm::cross(cameraPlayer.Front, cameraPlayer.Up)) * baseSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPlayer.Position += glm::normalize(glm::cross(cameraPlayer.Front, cameraPlayer.Up)) * baseSpeed;
}

inline void processInput(GLFWwindow *window, KP::OBJECT::Object& object, float deltaTime, float baseSpeed = 3.0f, float sprintSpeed = 5.0f, float walkSpeed = 1.5f, float acceleration = 10.0f, float friction = 8.0f) {
    if(flyMode){
        playerFly(window, object, deltaTime);
    }
    else {
        playerWalk(window, object, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        allObjects[sortedID[glock->UBO.objectId]].data.Position = glm::vec3(10.0f, 10.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
        allObjects[sortedID[glock->UBO.objectId]].data.Position = allObjects[sortedID[0]].data.Position;
    }
    
}

#endif // INPUT_H
