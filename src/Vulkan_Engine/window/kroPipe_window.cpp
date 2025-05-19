#include "kroPipe_window.hpp"
#include "../debug/kroPipe_debug.hpp" // Incluído para usar glfw_error_callback (se for mantido)

namespace KP {
namespace ENGINE {

// Definição da variável global do GLFWwindow (sem extern)
GLFWwindow* mWindow = nullptr;

// Definição do objeto Window (sem extern) - Fornecendo uma definição única
// Inicializado com valores padrão. Você pode querer um método de inicialização separado.
KP::ENGINE::Window OBJECT_window(800, 600, "KroPipe Window"); // Exemplo de definição e inicialização

} // namespace ENGINE
} // namespace KP

// Definição do construtor da classe Window
KP::ENGINE::Window::Window(int width, int height, const std::string& name)
    : width(width), height(height), name(name) {

    // O callback de erro do GLFW precisa ser registrado antes de glfwInit
    // Considere mover glfw_error_callback e seu registro para o ponto de inicialização do GLFW,
    // talvez na sua classe Instance ou um setup inicial.
    // glfwSetErrorCallback(KP::ENGINE::glfw_error_callback); // Se você decidir mantê-lo

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Não usar contexto OpenGL
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Permitir redimensionamento (necessário para recriar swapchain)

    // Criando a janela GLFW
    mWindow = glfwCreateWindow(this->width, this->height, name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(mWindow, this); // Associa este objeto Window à janela GLFW para callbacks

    // Associar o callback de redimensionamento (se estiver usando)
    // glfwSetFramebufferSizeCallback(mWindow, KP::ENGINE::Instance::framebufferResizeCallback); // Exemplo se callback estiver na Instance
}

// Definição do destrutor da classe Window
KP::ENGINE::Window::~Window() {
    if (mWindow) {
        glfwDestroyWindow(mWindow);
        mWindow = nullptr;
    }
    glfwTerminate(); // Termina GLFW apenas quando a última janela (ou apropriado)
}