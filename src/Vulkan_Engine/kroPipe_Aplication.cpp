#define STB_IMAGE_IMPLEMENTATION
#include "kroPipe_Aplication.hpp"
#include "debug/kroPipe_debug.hpp"
#include "instance/kroPipe_instance.hpp"
#include "window/kroPipe_window.hpp"
#include "window/kroPipe_windowSurface.hpp"

Aplication OBJECT_aplication;



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
    //KP::ENGINE::loadAllModels();
    KP::ENGINE::OBJECT_frameBuffer.createFrameBuffers();
    KP::ENGINE::createTextureImage();
    KP::ENGINE::createTextureImageView();
    KP::ENGINE::createTextureSampler();
    /*
    
    for(KP::ENGINE::Model* model : KP::ENGINE::allModel){
        model->UBO.create();    
    }  
    
    KP::ENGINE::OBJECT_command.createCommandBuffers();
    KP::ENGINE::OBJECT_render.createSyncObjects();
    */
    
    while(!glfwWindowShouldClose(KP::ENGINE::OBJECT_window.GLFW_window)) {
        
        glfwPollEvents();
    }

}


void Aplication::clean() {
    vkDestroyImageView(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::depthImageView, KP::ENGINE::VK_Allocator);
        vkDestroyImage(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::depthImage, KP::ENGINE::VK_Allocator);
        vkFreeMemory(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::depthImageMemory, KP::ENGINE::VK_Allocator);
        vkDestroySampler(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::textureSampler, KP::ENGINE::VK_Allocator);
        vkDestroyImageView(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::textureImageView, KP::ENGINE::VK_Allocator);
        vkDestroyImage(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::textureImage, KP::ENGINE::VK_Allocator);
        vkFreeMemory(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::textureImageMemory, KP::ENGINE::VK_Allocator);
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
    vkDestroySwapchainKHR(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::swapChain, KP::ENGINE::VK_Allocator);
    if(KP::ENGINE::debug){
        KP::ENGINE::OBJECT_debugger.DestroyDebugUtilsMessengerEXT(KP::ENGINE::VK_instance, debugMessenger, KP::ENGINE::VK_Allocator);
    }
    vkDestroySurfaceKHR(KP::ENGINE::VK_instance, KP::ENGINE::OBJECT_windowSurface.VK_surface, KP::ENGINE::VK_Allocator);
    vkDestroyDevice(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::VK_Allocator);
    vkDestroyInstance(KP::ENGINE::VK_instance, KP::ENGINE::VK_Allocator);
    //cleanPointers();
}

