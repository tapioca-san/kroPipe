#ifndef KROPIPE_SWAPCHAIN_HPP
#define KROPIPE_SWAPCHAIN_HPP

#include "../../kroPipe_depedence.hpp"
#include "../window/kroPipe_window.hpp" // Necessário para OBJECT_window, mWindow
#include "../device/kroPipe_device.hpp" // Necessário para OBJECT_device
#include "../queue/kroPipe_queuFamilies.hpp" // Necessário para QueueFamilyIndices
#include "../debug/kroPipe_debug.hpp" // Necessário para check_vk_result, err

#include "../buffers/kroPipe_frameBuffer.hpp" // Necessário para std::vector<VkFramebuffer>
#include "../depth/kroPipe_depth.hpp"       // Necessário para depthImageView, depthImage, depthImageMemory
#include "../image/kroPipe_imageView.hpp"   // Necessário para swapChainImageViews, swapChainImages



namespace KP {
namespace ENGINE {
extern std::vector<VkImage> swapChainImages; 
extern std::vector<VkImageView> swapChainImageViews; 

extern VkFormat swapChainImageFormat;
extern VkExtent2D swapChainExtent;
extern VkSwapchainKHR swapChain; 
extern std::vector<VkFramebuffer> swapChainFramebuffers; 
extern bool vsync; 

struct SwapChainSupportDetails{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain{

public:
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createSwapChain();
    void cleanupSwapChain();
    void recreateSwapChain();

};

extern KP::ENGINE::SwapChain OBJECT_swapChain;

} // namespace SWAPCHAIN
} // namespace KP

#endif //KROPIPE_SWAPCHAIN_HPP