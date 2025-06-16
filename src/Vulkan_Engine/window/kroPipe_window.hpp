#ifndef KROPIPE_WINDOW_HPP
#define KROPIPE_WINDOW_HPP

//#include "../swapchain/kroPipe_swapchain.hpp"
#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {
    
    
// Classe Window
class Window {
private:
    int width;
    int height;
    std::string name; 


public:
 
    GLFWwindow* GLFW_window; 


    
    Window(int width, int height, const std::string& name);
    ~Window();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const std::string& getName() const { return name; }
    GLFWwindow* getGlfwWindow() const { return GLFW_window; }

};

extern KP::ENGINE::Window OBJECT_window;

extern float lastX;
extern float lastY;
extern bool firstMouse;

} // namespace ENGINE
} // namespace KP

#endif//KROPIPE_WINDOW_HPP