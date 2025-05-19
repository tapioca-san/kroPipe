#include "kroPipe_windowSurface.hpp"
#include "../debug/kroPipe_debug.hpp"
#include <vulkan/vulkan_core.h>

void KP::ENGINE::WindowSurface::createSurface(VkInstance &instance, GLFWwindow *window, VkSurfaceKHR &surface) {
    

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error(fatalMessage("failed to create window surface!"));
    }
}

namespace KP {
namespace ENGINE {
KP::ENGINE::WindowSurface OBJECT_windowSurface; // amarzena o "ID" da janela gerado pelo glfw para podermos direcionar o vulkan renderizar só naquela tela, sem afetar outras janelas e sendo o mais direto o possível
} // namespace ENGINE
} // namespace KP