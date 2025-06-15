#ifndef INPUT_H
#define INPUT_H

#include "../../Vulkan_Engine/load/kroPipe_model.hpp"
#include "../object/kroPipe_object.hpp"
#include "../camera/kroPipe_camera.hpp"
#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace UTILS {


void processInput(GLFWwindow *window, KP::UTILS::Object& obj, float deltaTime, float baseSpeed = 3.0f, float sprintSpeed = 5.0f, float walkSpeed = 1.5f, float acceleration = 10.0f, float friction = 8.0f);

}// namespace UTILS
}// namespsace KP

#endif // INPUT_H
