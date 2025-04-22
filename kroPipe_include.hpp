#ifndef CONFIG_H
#define CONFIG_H

//#define NDEBUG


// debug mode
    #include <cstdint>
#ifdef NDEBUG
    const bool debug = false;
    #else
    const bool debug = true;
    #endif

    #ifdef NVSYNC
    const bool vsync = false;
    #else
    const bool vsync = true;
    #endif

    #ifdef NFRAMEPERSECOND
    const bool framePerSecond = false;
    #else
    const bool framePerSecond = true;
    #endif

    #ifdef NFLYMODE
    const bool flyMode = false;
    #else
    const bool flyMode = true;
    #endif
    
// 




// Importent API

    
//

// KROPIPE

    //#include "VKkropipe/kroPipe_ALL.hpp"
    #include "VKkropipe/kroPipe_camera.hpp"
//

// Starders API
    
//

// vulkan velidation layer
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
//

// VARIABLE CONFIGURATION
    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;

//

// VARIABLE
    bool framebufferResized = false;
    bool mouseDisable = true;

    float deltaTime;

    float lastTime = glfwGetTime();
    
    uint32_t imageIndex;
    

//

VkAllocationCallbacks*   g_Allocator = nullptr;
VkInstance               g_Instance = VK_NULL_HANDLE;
VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
VkDevice                 g_Device = VK_NULL_HANDLE;
uint32_t                 g_QueueFamily = (uint32_t)-1;
VkQueue                  g_Queue = VK_NULL_HANDLE;
VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

ImGui_ImplVulkanH_Window g_MainWindowData;
ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
uint32_t                 g_MinImageCount = 2;
bool                     g_SwapChainRebuild = false;

// VARIABLES to use anywhere

    VkAllocationCallbacks* Allocator = nullptr;

    VkDebugUtilsMessengerEXT debugMessenger;

    VkSurfaceKHR surface;
    
    VkQueue graphicsQueue;
    uint32_t graphicsIndex;
    
    VkShaderModule shaderModule;
    
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkSwapchainKHR swapChain;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    
    VkRenderPass renderPass;
    
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;
    
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkDescriptorSetLayout> setLayout;
    
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    
    VkMemoryRequirements memRequirements;
    VkPhysicalDeviceMemoryProperties memProperties;
    
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    
    VkImageView textureImageView;
    
    VkSampler textureSampler;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    
    //std::vector<Camera> camera;
    Camera cameraPlayer;
    

    VkResult err;
    

    std::string directoryFile = (std::string)std::filesystem::current_path() + "/VKconfiguration"; 
    std::string directoryFileManually = "";




//



#endif //CONFIG