
#define APP_NDEBUG
//#define APP_NVSYNC
//#define APP_NFRAMEPERSECOND
#define   APP_NFLYMODE

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
        KP::RENDER::OBJECT_render.drawFrame();
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


/*

function timeOfImpact(rect1, rect2) {
    const dvx = rect1.vx - rect2.vx;
    const dvy = rect1.vy - rect2.vy;

    if(dvx == 0 && dvy == 0) return false;

    let x1 = rect1.x + (rect2.width - rect1.width) / 2,
        y1 = rect1.y + (rect2.height - rect1.height) / 2,
        x2 = rect2.x,
        y2 = rect2.y;

    const tx1 = (x2 - x1 - rect1.width) / dvx;
    const tx2 = (x2 + rect2.width - x1) / dvx;
    const ty1 = (y2 - y1 - rect1.height) / dvy;
    const ty2 = (y2 + rect2.height - y1) / dvy;

    const minx = Math.min(tx1, tx2);
    const maxx = Math.max(tx1, tx2);
    const miny = Math.min(ty1, ty2);
    const maxy = Math.max(ty1, ty2);

    if(minx > maxy || miny > maxx || maxx < 0 || maxy < 0 || minx > 1 || miny > 1) return false;

    return Math.max(minx, miny, 0);
    // if it returns 0, it means they were already colliding at the beginning of the frame.
}

test case:

const rect1 = {
    x: 0,
    y: 0,
    width: 10,
    height: 10,
    vx: 1000,
    vy: 0
};

const rect2 = {
    x: 1015,
    y: 0,
    width: 20,
    height: 20,
    vx: -1000,
    vy: 0
};

console.log(timeOfImpact(rect1, rect2)); // 0.5
*/
