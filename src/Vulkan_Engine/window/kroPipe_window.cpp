#include "../instance/kroPipe_instance.hpp"
#include "../debug/kroPipe_debug.hpp" // Incluído para usar glfw_error_callback (se for mantido)
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
    glfwSetInputMode(GLFW_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// Definição do destrutor da classe Window
KP::ENGINE::Window::~Window() {
    if (GLFW_window) {
        glfwDestroyWindow(GLFW_window);
        GLFW_window = nullptr;
    }
    glfwTerminate(); 
}