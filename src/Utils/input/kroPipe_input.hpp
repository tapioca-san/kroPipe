#ifndef KROPIPE_INPUT_HPP
#define KROPIPE_INPUT_HPP

#include "../object/kroPipe_object.hpp"
#include "../../kroPipe_depedence.hpp"
#include <memory>

namespace KP {
namespace UTILS {


void processInput(GLFWwindow *window, std::shared_ptr<KP::UTILS::Object> obj, float deltaTime, float baseSpeed = 3.0f, float sprintSpeed = 5.0f, float walkSpeed = 1.5f, float acceleration = 10.0f, float friction = 8.0f);

extern bool ImguiMode;
extern bool mouseMode;

}// namespace UTILS
}// namespsace KP

#endif // KROPIPE_INPUT_HPP
