#ifndef INSTANCE_H
#define INSTANCE_H

// adaptar o instance para o novo modo de intanciar objetos do modelos e fazer com que o codigo de desenhar na tela pelo comando do kroPipe_command seja adaptado dentro do sistema de model e que o objeto do kroPipe_object já venha com carregamento de modelo

#include "../VKkropipe/kroPipe_texture.hpp"
#include "../VKkropipe/kroPipe_model.hpp"
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


#ifndef EXTENSION_H
    #include "kroPipe_extension.hpp"
#endif


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
        infoMensage("Pointers has been cleaned");
    }

    public:
    // LOCAL VARIABLE
    
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
    
    //
    
    Instance(){
            // CALLBACK
                glfwSetFramebufferSizeCallback(mWindow, framebufferResizeCallback);
            //

            // FUNCTIONS
            
            createInstace();
            //setAllObjectNames();
            KP::DEBUG::setupDebugMessenger(instance, debugMessenger);
            KP::WINDOWSURFACE::createSurface(instance, mWindow);
            KP::DEVICE::pickPhysicalDevice(instance);
            KP::DEVICE::createLogicalDevice();
            KP::SWAPCHAIN::createSwapChain();
            KP::IMAGEVIEW::createImageViews();
            KP::PIPELINE::createRenderPass();    
            for(KP::OBJECT::Model* model : KP::OBJECT::allModel){
                model->UBO.createDescriptorLayout();    
            }  
            KP::PIPELINE::createGraphicsPipeline();  
            KP::COMMANDBUFFER::createCommandPool();
            KP::DEPTH::createDepthResources();
            KP::OBJECT::loadAllModels();
            KP::FRAMEBUFFER::createFrameBuffers();
            KP::TEXTURE::createTextureImage();
            KP::TEXTURE::createTextureImageView();
            KP::TEXTURE::createTextureSampler();
            //createVertexBuffer();
            //createIndexBuffer();
            for(KP::OBJECT::Model* model : KP::OBJECT::allModel){
                model->UBO.create();    
            }  
            KP::COMMANDBUFFER::createCommandBuffers();
            KP::RENDER::createSyncObjects();
            //
        }
        
        ~Instance(){
            vkDestroyImageView(device, depthImageView, nullptr);
            vkDestroyImage(device, depthImage, nullptr);
            vkFreeMemory(device, depthImageMemory, nullptr);

            vkDestroySampler(device, textureSampler, nullptr);
            vkDestroyImageView(device, textureImageView, nullptr);
            vkDestroyImage(device, textureImage, nullptr);
            vkFreeMemory(device, textureImageMemory, nullptr);
            for (KP::OBJECT::Model *&model: KP::OBJECT::allModel) {
                model->UBO.cleanUp();
                model->cleanupVao();
            }
            KP::RENDER::destroyRender();
            KP::COMMANDBUFFER::destroyCommandPool();
            KP::FRAMEBUFFER::CleanUpFramerBuffer();
            KP::PIPELINE::CleanUpPipeline();
            KP::IMAGEVIEW::DestroyImageview();
            vkDestroySwapchainKHR(device, swapChain, nullptr);
            if(debug){
                KP::DEBUG::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
            }
            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyDevice(device, nullptr);
            vkDestroyInstance(instance, nullptr);
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
                    
                    KP::DEBUG::populateDebugMessengerCreateInfo(debugCreateInfo);
                    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
                    
                } else {

                    createInfo.pNext = nullptr;
                }
                
            //   

            
            // making an instance with instance configuration that we've done. this must be the final step of instance
                if(vkCreateInstance(&createInfo,nullptr,&instance) != VK_SUCCESS){
                    throw std::runtime_error(fatalMensage("failed to create instance!\n"));
                }
            //
        }
     

        // before active our validation layer debug, we must check if that exist on our computer.
            bool checkValidationLayerSupport(){
                uint32_t layerCount;
                vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
                
                std::vector<VkLayerProperties> avaibleLayers(layerCount);

                vkEnumerateInstanceLayerProperties(&layerCount, avaibleLayers.data());
                
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