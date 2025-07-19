#include "../../Vulkan_Engine/window/kroPipe_window.hpp"
#include "../../Utils/camera/kroPipe_camera.hpp"
#include "../object/kroPipe_object.hpp"
#include "kroPipe_input.hpp"
namespace KP {
namespace UTILS {


bool flyMode = true;
bool ImguiMode = false; // stander
bool mouseMode = false; // stander

void playerWalk(GLFWwindow *window, std::shared_ptr<KP::UTILS::Object> object, float deltaTime, float baseSpeed, float sprintSpeed, float walkSpeed, float acceleration, float friction) {

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    baseSpeed = sprintSpeed;
  } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    baseSpeed = walkSpeed;
  }

  glm::vec3 inputDir(0.0f);

  if (!object->getData().is_OnAir) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      inputDir.z += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      inputDir.z -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      inputDir.x -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      inputDir.x += 1.0f;

    if (glm::length(inputDir) > 0.0f) {
      inputDir = glm::normalize(inputDir);

      glm::vec3 forward = cameraPlayer.Front;
      forward = glm::normalize(forward);
      glm::vec3 right = glm::normalize(glm::cross(forward, cameraPlayer.Up));

      glm::vec3 moveDir = inputDir.z * forward + inputDir.x * right;

      object->getData().velocity += moveDir * acceleration * deltaTime;
    } else {
      object->getData().velocity.x =
          glm::mix(object->getData().velocity.x, 0.0f, friction * deltaTime);
      object->getData().velocity.z =
          glm::mix(object->getData().velocity.z, 0.0f, friction * deltaTime);
    }
    glm::vec2 horizontalVel(object->getData().velocity.x,
                            object->getData().velocity.z);
    float speed = glm::length(horizontalVel);
    if (speed > baseSpeed) {
      horizontalVel = glm::normalize(horizontalVel) * baseSpeed;
      object->getData().velocity.x = horizontalVel.x;
      object->getData().velocity.z = horizontalVel.y;
    }
  }

  object->getData().Position += glm::vec3(object->getData().velocity.x, 0.0f, object->getData().velocity.z) *deltaTime;

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !object->getData().is_OnAir) {
    object->getData().velocity.y = 5.0f;
    object->getData().is_OnAir = true;
  }
}

void playerFly(GLFWwindow *window, std::shared_ptr<KP::UTILS::Object> object, float deltaTime,
               float baseSpeed, float sprintSpeed, float walkSpeed) {

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    baseSpeed = sprintSpeed;
  } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    baseSpeed = walkSpeed;
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    baseSpeed = sprintSpeed;
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    baseSpeed = walkSpeed;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPlayer.Position += baseSpeed * cameraPlayer.Front * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPlayer.Position -= baseSpeed * cameraPlayer.Front * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPlayer.Position -= glm::normalize(glm::cross(cameraPlayer.Front, cameraPlayer.Up)) * baseSpeed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPlayer.Position += glm::normalize(glm::cross(cameraPlayer.Front, cameraPlayer.Up)) * baseSpeed * deltaTime;
}

void processInput(GLFWwindow *window, std::shared_ptr<KP::UTILS::Object> object, float deltaTime) {
 
  playerWalk(window, object, deltaTime);
  
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
    KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0))->getData().Position = glm::vec3(10.0f, 111.0f, 0.0f);
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

} // namespace UTILS
} // namespace KP
