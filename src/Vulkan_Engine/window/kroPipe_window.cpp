#include "../../Utils/camera/kroPipe_camera.hpp"
#include "../instance/kroPipe_instance.hpp"
#include "../render/kroPipe_render.hpp"
#include "../debug/kroPipe_debug.hpp" 
#include "kroPipe_windowSurface.hpp"
#include "kroPipe_window.hpp"

namespace KP {
namespace ENGINE {

KP::ENGINE::Window OBJECT_window(800, 600, "KroPipe Window"); 

float lastX = 0;
float lastY = 0;
bool firstMouse = true;

void framebufferResizeCallback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

} // namespace ENGINE
} // namespace KP

KP::ENGINE::Window::Window(int width, int height, const std::string& name)
    : width(width), height(height), name(name) {

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); 
    GLFW_window = glfwCreateWindow(this->width, this->height, name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(GLFW_window, this); // Associa este objeto Window à janela GLFW para callbacks
    
    { //config

        glfwSetInputMode(GLFW_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetFramebufferSizeCallback(KP::ENGINE::OBJECT_window.getGlfwWindow(), framebufferResizeCallback);
        glfwSetCursorPosCallback(GLFW_window, mouse_callback);
        glfwSetScrollCallback(GLFW_window, scroll_callback);
    }
}

// Definição do destrutor da classe Window
KP::ENGINE::Window::~Window() {
    if (GLFW_window) {
        glfwDestroyWindow(GLFW_window);
        GLFW_window = nullptr;
    }
    glfwTerminate(); 
}

void KP::ENGINE::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (KP::ENGINE::firstMouse)
    {
        KP::ENGINE::lastX = xpos;
        KP::ENGINE::lastY = ypos;
        KP::ENGINE::firstMouse = false;
    }

    float xoffset = xpos - KP::ENGINE::lastX;
    float yoffset = KP::ENGINE::lastY - ypos; // reversed since y-coordinates go from bottom to top

    KP::ENGINE::lastX = xpos;
    KP::ENGINE::lastY = ypos;

    KP::UTILS::cameraPlayer.ProcessMouseMovement(xoffset, yoffset);
}

void KP::ENGINE::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    KP::UTILS::cameraPlayer.ProcessMouseScroll(static_cast<float>(yoffset));
}

void KP::ENGINE::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    KP::ENGINE::framebufferResized = true;
}