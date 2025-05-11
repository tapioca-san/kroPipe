#ifndef INSTANCE_H
#define INSTANCE_H

// adaptar o instance para o novo modo de intanciar objetos do modelos e fazer com que o codigo de desenhar na tela pelo comando do kroPipe_command seja adaptado dentro do sistema de model e que o objeto do kroPipe_object já venha com carregamento de modelo

#include "../VKkropipe/kroPipe_texture.hpp"
#include "../VKkropipe/kroPipe_model.hpp"
#include "../VKkropipe/kroPipe_ImGui.hpp"
#include "../VKkropipe/kroPipe_Log.hpp"
#include "kroPipe_windowSurface.hpp"
#include "kroPipe_frameBuffer.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_swapchain.hpp"
#include "kroPipe_imageView.hpp"
#include "kroPipe_pipeline.hpp"
#include "kroPipe_command.hpp"
#include "kroPipe_window.hpp"
#include "kroPipe_device.hpp"
#include "kroPipe_render.hpp"
#include "kroPipe_buffer.hpp"
#include "kroPipe_debug.hpp"
#include "kroPipe_depth.hpp"
//#include "kroPipe_name.hpp"




// GLOBAL VARIABLE
//

class Instance{
    
    
    private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Instance*>(glfwGetWindowUserPointer(window));
        framebufferResized = true;
    }
    
    void cleanPointers(){   //anti memory leak

        for(KP::OBJECT::Model* model : KP::OBJECT::allModel){
            delete model;
        }
        KP::OBJECT::allModel.clear();
        for (KP::OBJECT::Object *obj : allObjects) {
            delete obj;
        }
        allObjects.clear();
        infoMensage("Pointers has been cleaned");
    }

    public:
    
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
    
    //
    
    Instance(){
            // CALLBACK
            glfwSetFramebufferSizeCallback(mWindow, framebufferResizeCallback);
            //

            // FUNCTIONS
            //setAllObjectNames();
          
            createInstace();
            { // INIT VULKAN BASIC THINGS
                KP::DEBUG::OBJECT_debug.setupDebugMessenger(g_Instance, debugMessenger);
                KP::WINDOWSURFACE::OBJECT_windowSurface.createSurface(g_Instance, mWindow);
                KP::DEVICE::OBJECT_device.pickPhysicalDevice(g_Instance);
                KP::DEVICE::OBJECT_device.createLogicalDevice();
                KP::SWAPCHAIN::OBJECT_swapChain.createSwapChain();
                KP::IMAGEVIEW::OBJECT_imageView.createImageViews();
                KP::PIPELINE::OBJECT_pipeline.createRenderPass();    
            }
            { // LOAD ALL MODELS THROUGH //VKkropipe/kroPipe_model.hpp
                for(KP::OBJECT::Model* model : KP::OBJECT::allModel){
                    model->UBO.createDescriptorLayout();    
                }  
            }
            { // INIT VULKAN BASIC THINGS
                KP::PIPELINE::OBJECT_pipeline.createGraphicsPipeline();  
                KP::COMMANDBUFFER::OBJECT_command.createCommandPool();
                KP::DEPTH::OBJECT_depth.createDepthResources();
                KP::OBJECT::loadAllModels(); // Load every
            }
            
            KP::FRAMEBUFFER::OBJECT_frameBuffer.createFrameBuffers();
            KP::TEXTURE::createTextureImage();
            KP::TEXTURE::createTextureImageView();
            KP::TEXTURE::createTextureSampler();
            //createVertexBuffer();
            //createIndexBuffer();
            for(KP::OBJECT::Model* model : KP::OBJECT::allModel){
                model->UBO.create();    
            }  
            KP::COMMANDBUFFER::OBJECT_command.createCommandBuffers();
            KP::RENDER::OBJECT_render.createSyncObjects();
            
            { // create interface 2d to manage engine grafics
                imguiInterface = new KP::IMGUI::Imgui(
                    mWindow,g_Instance, g_PhysicalDevice, g_Device,
                    KP::QUEUFAMILIES::OBJECT_queuFamilies.presentQueue, g_QueueFamily, g_PipelineCache,
                    renderPass
                );
            }
                
            
            //
        }
        
        ~Instance(){
            imguiInterface->cleanup();
            vkDestroyImageView(g_Device, depthImageView, Allocator);
            vkDestroyImage(g_Device, depthImage, Allocator);
            vkFreeMemory(g_Device, depthImageMemory, Allocator);

            vkDestroySampler(g_Device, textureSampler, Allocator);
            vkDestroyImageView(g_Device, textureImageView, Allocator);
            vkDestroyImage(g_Device, textureImage, Allocator);
            vkFreeMemory(g_Device, textureImageMemory, Allocator);
            for (KP::OBJECT::Model *&model: KP::OBJECT::allModel) {
                model->UBO.cleanUp();
                model->cleanupVao();
            }
            KP::RENDER::OBJECT_render.destroyRender();
            KP::COMMANDBUFFER::OBJECT_command.destroyCommandPool();
            KP::FRAMEBUFFER::OBJECT_frameBuffer.CleanUpFramerBuffer();
            KP::PIPELINE::OBJECT_pipeline.CleanUpPipeline();
            KP::IMAGEVIEW::OBJECT_imageView.DestroyImageview();
            vkDestroySwapchainKHR(g_Device, swapChain, Allocator);
            if(debug){
                KP::DEBUG::OBJECT_debug.DestroyDebugUtilsMessengerEXT(g_Instance, debugMessenger, Allocator);
            }
            vkDestroySurfaceKHR(g_Instance, surface, Allocator);
            vkDestroyDevice(g_Device, Allocator);
            vkDestroyInstance(g_Instance, Allocator);
            cleanPointers();

        }
        
        
        void createInstace(){
            if(debug && !checkValidationLayerSupport()){
                throw std::runtime_error(fatalMensage("debug requested, but not available!\n"));
            }
            // instance's configuration
            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Anork";
            appInfo.pEngineName = "KROPIPE";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;
                    
            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            //
            
            // active basic extension from GLFW
                auto extensions = getRequiredExtensions();
                
                createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
                createInfo.ppEnabledExtensionNames = extensions.data();
                //
                
                // validation layers
                VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
                if(debug){
                    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                    createInfo.ppEnabledLayerNames = validationLayers.data();
                    
                    KP::DEBUG::OBJECT_debug.populateDebugMessengerCreateInfo(debugCreateInfo);
                    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
                    
                } else {

                    createInfo.pNext = nullptr;
                }
                
            //   

            
            // making an instance with instance configuration that we've done. this must be the final step of instance
                err = vkCreateInstance(&createInfo,Allocator,&g_Instance);
                check_vk_result(err, "failed to create instance!\n");
            //
        }
     

        // before active our validation layer debug, we must check if that exist on our computer.
            bool checkValidationLayerSupport(){
                uint32_t layerCount;
                vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
                
                std::vector<VkLayerProperties> avaibleLayers(layerCount);

                err = vkEnumerateInstanceLayerProperties(&layerCount, avaibleLayers.data());
                check_vk_result(err);
                
                for (const char* layerName : validationLayers){
                    bool layerFound = false;

                    for (const auto& layerProperties : avaibleLayers){
                        if(strcmp(layerName,layerProperties.layerName) == 0){
                            layerFound = true;
                            break;
                        }
                    }
                    if (!layerFound) {
                        return false;
                    }
                }
                
                return true;
            }
        //
        
        // get all extensions to work with vulkan :)
            std::vector<const char*> getRequiredExtensions() {
                uint32_t glfwExtensionCount = 0;
                const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            
                std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
            
                if (debug) {
                    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                }
            
                return extensions;
            }
        //

};

#endif //INSTANCE_H