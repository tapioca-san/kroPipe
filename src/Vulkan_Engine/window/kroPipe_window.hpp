#ifndef KROPIPE_WINDOW_HPP
#define KROPIPE_WINDOW_HPP

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {

// Variável global do GLFWwindow (declarada extern)
extern GLFWwindow* mWindow;

// Classe Window
class Window {
private:
    int width;
    int height;
    std::string name; // Adicionado std::string

public:
    VkSurfaceKHR VK_surface = VK_NULL_HANDLE; // Inicializado para garantir estado conhecido

    Window(int width, int height, const std::string& name);
    ~Window();

    // Métodos para obter propriedades da janela
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const std::string& getName() const { return name; }
    GLFWwindow* getGlfwWindow() const { return mWindow; }

};

// Objeto Window (declarado extern)
extern KP::ENGINE::Window OBJECT_window;

} // namespace ENGINE
} // namespace KP

#endif//KROPIPE_WINDOW_HPP