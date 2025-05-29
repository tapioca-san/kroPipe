#define STB_IMAGE_IMPLEMENTATION
#include "kroPipe_Aplication.hpp"
#include "debug/kroPipe_debug.hpp"
#include "instance/kroPipe_instance.hpp"
#include "window/kroPipe_window.hpp"
#include "window/kroPipe_windowSurface.hpp"

Aplication OBJECT_aplication;



void Aplication::init(){

    KP::ENGINE::OBJECT_instance.createInstance(); // Chama o método na instância local

    // Usando OBJECT_debugger e VK_instance do namespace, e debugMessenger (membro da classe Aplication)
    KP::ENGINE::OBJECT_debugger.setupDebugMessenger(KP::ENGINE::VK_instance, debugMessenger);

    KP::ENGINE::OBJECT_windowSurface.createSurface(KP::ENGINE::VK_instance, KP::ENGINE::OBJECT_window.GLFW_window, KP::ENGINE::OBJECT_windowSurface.VK_surface);
    
    // Usando OBJECT_device e VK_instance do namespace
    KP::ENGINE::OBJECT_device.pickPhysicalDevice(KP::ENGINE::VK_instance);

    // Usando OBJECT_device do namespace
    KP::ENGINE::OBJECT_device.createLogicalDevice();


    KP::ENGINE::OBJECT_swapChain.createSwapChain();
    KP::ENGINE::OBJECT_imageView.createImageViews();
    KP::ENGINE::OBJECT_pipeline.createRenderPass();    
    { // LOAD ALL MODELS THROUGH //VKkropipe/kroPipe_model.hpp
        for(KP::ENGINE::Model* model : KP::ENGINE::allModel){
            model->UBO.createDescriptorLayout(); // this function set everything on setLayouts to use to graphicsPipeline under here
        }  
    }
    
    KP::ENGINE::OBJECT_pipeline.createGraphicsPipeline();  
    KP::ENGINE::OBJECT_command.createCommandPool();
    KP::ENGINE::OBJECT_depth.createDepthResources();
    //KP::OBJECT::loadAllModels(); // Load every
    while(!glfwWindowShouldClose(KP::ENGINE::OBJECT_window.GLFW_window)) { // render frame by frame
        
        glfwPollEvents();
    }

}

// Adicione outros métodos da classe Aplication (run, cleanup, etc.) aqui

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

