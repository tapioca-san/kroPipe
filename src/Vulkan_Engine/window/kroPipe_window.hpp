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

    float lastX = 0;
    float lastY = 0;
    bool firstMouse = true;

public:
 
    GLFWwindow* GLFW_window; 


    
    Window(int width, int height, const std::string& name);
    ~Window();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const std::string& getName() const { return name; }
    GLFWwindow* getGlfwWindow() const { return GLFW_window; }

    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

};

extern KP::ENGINE::Window OBJECT_window;

} // namespace ENGINE
} // namespace KP

#endif//KROPIPE_WINDOW_HPP