#include "../../Utils/camera/kroPipe_camera.hpp"
#include "../instance/kroPipe_instance.hpp"
#include "../debug/kroPipe_debug.hpp" 
#include "kroPipe_windowSurface.hpp"
#include "kroPipe_window.hpp"

namespace KP {
namespace ENGINE {


KP::ENGINE::Window OBJECT_window(800, 600, "KroPipe Window"); 

} // namespace ENGINE
} // namespace KP

KP::ENGINE::Window::Window(int width, int height, const std::string& name)
    : width(width), height(height), name(name) {

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); 
    GLFW_window = glfwCreateWindow(this->width, this->height, name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(GLFW_window, this); // Associa este objeto Window à janela GLFW para callbacks
    //glfwSetInputMode(GLFW_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// Definição do destrutor da classe Window
KP::ENGINE::Window::~Window() {
    if (GLFW_window) {
        glfwDestroyWindow(GLFW_window);
        GLFW_window = nullptr;
    }
    glfwTerminate(); 
}

void KP::ENGINE::Window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    KP::UTILS::cameraPlayer.ProcessMouseMovement(xoffset, yoffset);
}

void KP::ENGINE::Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    KP::UTILS::cameraPlayer.ProcessMouseScroll(static_cast<float>(yoffset));
}