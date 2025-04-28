
//#define NDEBUG
//#define NVSYNC
//#define NFRAMEPERSECOND
#define NFLYMODE
//https://github.com/ocornut/imgui/blob/master/examples/example_glfw_vulkan/main.cpp

#define STB_IMAGE_IMPLEMENTATION
#include "VKconfiguration/kroPipe_window.hpp"
#include "VKkropipe/kroPipe_gravity.hpp"
#include "VKInitConfig/kroPipe_init.hpp"
#include "VKkropipe/kroPipe_entity.hpp"
#include "VKkropipe/kroPipe_info.hpp"
#include "kroPipe_include.hpp"
#include "kroPipe_engine.hpp"

// Variable
    const char* nameWindow = "triangle";

    const uint32_t width = 1200;
    const uint32_t height = 1200;

    float lastX = swapChainExtent.width / 2.0f;
    float lastY = swapChainExtent.height / 2.0f;
    bool firstMouse = true;

    FPSCounter info;
//


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main(){

    glfwSetErrorCallback(glfw_error_callback);
    makeConfiguration(); // init for a best configuratiopn coming from the user.

    window window(width, height, nameWindow);
    Instance myinstance;
    createEntity(cameraPlayer.Position + glm::vec3(1.0f,1.0f,1.0f), 0.0f,true);
    //create
    loadObjects(sortedID);
    

    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    while(!glfwWindowShouldClose(mWindow)) {
        
        glfwPollEvents();

        imguiInterface->newFrame(); 
        imguiInterface->drawWindows();         
        ImGui::Render(); 
        


        for(uint16_t i = 0; i < allObjects.size(); i++){
            kroPipe::gravityForce(allObjects[sortedID[i]], deltaTime);
        }
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if(framePerSecond){
            info.update();
        }
        
        processInput(mWindow, allObjects[sortedID[0]], deltaTime);
        KP::RENDER::drawFrame();
    }

    vkDeviceWaitIdle(g_Device);
    return 0;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){   
    if(cameraPlayer.cameraWork){
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        cameraPlayer.ProcessMouseMovement(xoffset, yoffset);
    }    
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if(cameraPlayer.cameraWork){
        cameraPlayer.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}
