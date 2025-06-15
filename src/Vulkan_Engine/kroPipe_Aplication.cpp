
#define STB_IMAGE_IMPLEMENTATION
#include "kroPipe_Aplication.hpp"

#include "../Utils/camera/kroPipe_camera.hpp"
#include "../Utils/object/kroPipe_object.hpp"
#include "window/kroPipe_windowSurface.hpp"
#include "../Utils/input/kroPipe_input.hpp"
#include "../Utils/imgui/kroPipe_imgui.hpp"
#include "window/kroPipe_windowSurface.hpp"
#include "buffers/kroPipe_frameBuffer.hpp"
#include "swapchain/kroPipe_swapchain.hpp"
#include "queue/kroPipe_queuFamilies.hpp"
#include "pipeline/kroPipe_pipeline.hpp"
#include "instance/kroPipe_instance.hpp"
#include "instance/kroPipe_instance.hpp"
#include "texture/kroPipe_texture.hpp"
#include "image/kroPipe_imageView.hpp"
#include "command/kroPipe_command.hpp"
#include "window/kroPipe_window.hpp"
#include "window/kroPipe_window.hpp"
#include "render/kroPipe_render.hpp"
#include "device/kroPipe_device.hpp"
#include "depth/kroPipe_depth.hpp"
#include "debug/kroPipe_debug.hpp" 
#include "load/kroPipe_model.hpp"

Aplication OBJECT_aplication;


float deltaTime;
float lastTime = glfwGetTime();

void Aplication::init(){

    KP::ENGINE::OBJECT_instance.createInstance();
    KP::ENGINE::OBJECT_debugger.setupDebugMessenger(KP::ENGINE::VK_instance, debugMessenger);
    KP::ENGINE::OBJECT_windowSurface.createSurface(KP::ENGINE::VK_instance, KP::ENGINE::OBJECT_window.GLFW_window, KP::ENGINE::OBJECT_windowSurface.VK_surface);
    KP::ENGINE::OBJECT_device.pickPhysicalDevice(KP::ENGINE::VK_instance);
    KP::ENGINE::OBJECT_device.createLogicalDevice();


    KP::ENGINE::OBJECT_swapChain.createSwapChain();
    KP::ENGINE::OBJECT_imageView.createImageViews();
    KP::ENGINE::OBJECT_pipeline.createRenderPass();    
    {
        for(KP::ENGINE::Model* model : KP::ENGINE::allModel){
            model->UBO.createDescriptorLayout();
            }  
    }
    
    KP::ENGINE::OBJECT_pipeline.createGraphicsPipeline();  
    KP::ENGINE::OBJECT_command.createCommandPool();
    KP::ENGINE::OBJECT_depth.createDepthResources();
    KP::ENGINE::loadAllModels();
    KP::ENGINE::OBJECT_frameBuffer.createFrameBuffers();
    KP::ENGINE::createTextureImage();
    KP::ENGINE::createTextureImageView();
    KP::ENGINE::createTextureSampler();
    
    for(KP::ENGINE::Model* model : KP::ENGINE::allModel){
        model->UBO.create();    
    }  
    
    KP::ENGINE::OBJECT_command.createCommandBuffers();
    KP::ENGINE::OBJECT_render.createSyncObjects();
    
    KP::ENGINE::imguiInterface = new KP::ENGINE::Imgui(
        KP::ENGINE::OBJECT_window.GLFW_window, KP::ENGINE::VK_instance, KP::ENGINE::OBJECT_device.getPhysicalDevice(), KP::ENGINE::OBJECT_device.getDevice(),
        KP::ENGINE::OBJECT_queuFamilies.presentQueue, KP::ENGINE::OBJECT_queuFamilies.graphicsIndex, KP::ENGINE::PipelineCache,
        KP::ENGINE::VK_renderPass
    );
    
    KP::UTILS::createEntity(KP::ENGINE::cameraPlayer.Position + glm::vec3(1.0f,1.0f,1.0f), 0.0f, true);
    KP::UTILS::loadObjects(KP::UTILS::sortedID);
    //create

}


void Aplication::run(){
    while(!glfwWindowShouldClose(KP::ENGINE::OBJECT_window.GLFW_window)) {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        glfwPollEvents();
        KP::UTILS::processInput(KP::ENGINE::OBJECT_window.getGlfwWindow(), *KP::UTILS::allObjects[KP::UTILS::sortedID[0]], deltaTime);
        
        KP::ENGINE::imguiInterface->newFrame(); 
        KP::ENGINE::imguiInterface->drawWindows();         
        ImGui::Render(); 


        //for(uint16_t i = 0; i < allObjects.size(); i++){
        //    kroPipe::gravityForce(allObjects[sortedID[i]], deltaTime);
        //db}

        KP::ENGINE::OBJECT_render.drawFrame();
    }
}

void Aplication::clean() {
    vkDeviceWaitIdle(KP::ENGINE::OBJECT_device.getDevice());
    KP::ENGINE::imguiInterface->cleanup();
    vkDestroyImageView(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::depthImageView, KP::ENGINE::VK_Allocator);
    vkDestroyImage(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::depthImage, KP::ENGINE::VK_Allocator);
    vkFreeMemory(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::depthImageMemory, KP::ENGINE::VK_Allocator);
    vkDestroySampler(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::textureSampler, KP::ENGINE::VK_Allocator);
    vkDestroyImageView(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::textureImageView, KP::ENGINE::VK_Allocator);
    vkDestroyImage(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::textureImage, KP::ENGINE::VK_Allocator);
    vkFreeMemory(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::textureImageMemory, KP::ENGINE::VK_Allocator);
    for (KP::ENGINE::Model *&model: KP::ENGINE::allModel) {
        model->UBO.cleanUp();
        model->cleanupVao();
    }
    KP::ENGINE::OBJECT_render.destroyRender();
        
    //KP::LIGHT::OBJECT_light.cleanUp();
    KP::ENGINE::OBJECT_command.destroyCommandPool();
    KP::ENGINE::OBJECT_frameBuffer.CleanUpFramerBuffer();
    KP::ENGINE::OBJECT_pipeline.CleanUpPipeline();
    KP::ENGINE::OBJECT_imageView.DestroyImageview();
    vkDestroySwapchainKHR(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::swapChain, KP::ENGINE::VK_Allocator);
    if(KP::ENGINE::debug){
        KP::ENGINE::OBJECT_debugger.DestroyDebugUtilsMessengerEXT(KP::ENGINE::VK_instance, debugMessenger, KP::ENGINE::VK_Allocator);
    }
    vkDestroySurfaceKHR(KP::ENGINE::VK_instance, KP::ENGINE::OBJECT_windowSurface.VK_surface, KP::ENGINE::VK_Allocator);
    vkDestroyDevice(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::VK_Allocator);
    vkDestroyInstance(KP::ENGINE::VK_instance, KP::ENGINE::VK_Allocator);
    //cleanPointers();
}

