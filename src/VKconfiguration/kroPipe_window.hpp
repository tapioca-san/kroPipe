#ifndef WINDOW_H
#define WINDOW_H

#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"

GLFWwindow* mWindow;

class window{

    private:
    
    int width;
    int height; 
    
    float lastX = swapChainExtent.width / 2.0f;
    float lastY = swapChainExtent.height / 2.0f;    
    bool firstMouse = true; 

    


    public:
    
    
    std::string name;
    
    window(int width, int height, std::string name) : width(width), height(height), name(name){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        mWindow = glfwCreateWindow(this->width, this->height, name.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(mWindow);
        glfwSetWindowUserPointer(mWindow, this);

        // tell GLFW to disable cursor
        glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    ~window(){
        glfwDestroyWindow(mWindow);
        glfwTerminate();
    }
};


#endif //WINDOW_H