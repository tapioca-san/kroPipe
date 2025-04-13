#ifndef WINDOWSURFACE_H
#define WINDOWSURFACE_H

#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"

namespace KP {
namespace WINDOWSURFACE {

void createSurface(VkInstance &instance, GLFWwindow *window) {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error(fatalMensage("failed to create window surface!"));
    }
}

}//WINDOWSURFACE
}//KP

#endif