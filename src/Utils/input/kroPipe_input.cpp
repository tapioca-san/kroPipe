#include "../../Utils/camera/kroPipe_camera.hpp"
#include "../load/kroPipe_model.hpp"
#include "kroPipe_input.hpp"


namespace KP {
namespace UTILS {

void processInput(GLFWwindow *window, KP::UTILS::Object& obj, float deltaTime, float baseSpeed, float sprintSpeed, float walkSpeed, float acceleration, float friction) {
    float targetSpeed = baseSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        targetSpeed = sprintSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        targetSpeed = walkSpeed;
    }

    glm::vec3 inputDir(0.0f);

    if (!obj.data.is_OnAir) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) inputDir.z += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) inputDir.z -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) inputDir.x -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) inputDir.x += 1.0f;

        if (glm::length(inputDir) > 0.0f) {
            inputDir = glm::normalize(inputDir);

            glm::vec3 forward = KP::UTILS::cameraPlayer.Front;
            forward.y = 0.0f;
            forward = glm::normalize(forward);
            glm::vec3 right = glm::normalize(glm::cross(forward, KP::UTILS::cameraPlayer.Up));

            glm::vec3 moveDir = inputDir.z * forward + inputDir.x * right;

            obj.data.velocity += moveDir * acceleration * deltaTime;
        } else {
            obj.data.velocity.x = glm::mix(obj.data.velocity.x, 0.0f, friction * deltaTime);
            obj.data.velocity.z = glm::mix(obj.data.velocity.z, 0.0f, friction * deltaTime);
        }
        glm::vec2 horizontalVel(obj.data.velocity.x, obj.data.velocity.z);
        float speed = glm::length(horizontalVel);
        if (speed > targetSpeed) {
            horizontalVel = glm::normalize(horizontalVel) * targetSpeed;
            obj.data.velocity.x = horizontalVel.x;
            obj.data.velocity.z = horizontalVel.y;
        }
    }

    obj.data.Position += glm::vec3(obj.data.velocity.x, 0.0f, obj.data.velocity.z) * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !obj.data.is_OnAir) {
        obj.data.velocity.y = 5.0f;
        obj.data.is_OnAir = true;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        obj.data.Position = glm::vec3(10.0f, 10.0f, 0.0f);
    }

     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        ImguiMode = true;
        mouseMode = true;
        glfwSetInputMode(KP::ENGINE::OBJECT_window.getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
     }
     if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
        ImguiMode = false;
        mouseMode = false;
        glfwSetInputMode(KP::ENGINE::OBJECT_window.getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
     }

}

bool ImguiMode = false; // stander
bool mouseMode = false; // stander

}// namespace UTILS
}// namespsace KP
