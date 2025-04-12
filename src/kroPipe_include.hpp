#ifndef CONFIG_H
#define CONFIG_H

//#define NDEBUG


// debug mode
    #include "VKkropipe/kroPipe_object.hpp"
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
// 




// Importent API

    #include "kroPipe_depedence.hpp"
    
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
    
    std::vector<KP::OBJECT::Object> allObjects;
    std::vector<int> sortedID;
    
    uint32_t imageIndex;
    
    int lastID = 0;

//

// VARIABLES to use anywhere

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkSurfaceKHR surface;
    
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    
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

    VkDescriptorPool descriptorPool;

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
    

    std::string directoryFile = (std::string)std::filesystem::current_path() + "/VKconfiguration"; 
    std::string directoryFileManually = "";


    


//



#endif //CONFIG