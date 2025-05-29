#include "kroPipe_windowSurface.hpp"
#include "../debug/kroPipe_debug.hpp"

void KP::ENGINE::WindowSurface::createSurface(VkInstance &instance, GLFWwindow *window, VkSurfaceKHR &surface) {
    

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error(fatalMessage("failed to create window surface!"));
    }
}

namespace KP {
namespace ENGINE {
KP::ENGINE::WindowSurface OBJECT_windowSurface;
} // namespace ENGINE
} // namespace KP