/*

parei na parte "Criando a corrente de troca"

*/


#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "../VKkropipe/kroPipe_Log.hpp"
#include "kroPipe_queuFamilies.hpp"
#include "kroPipe_frameBuffer.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_imageView.hpp"
#include "kroPipe_window.hpp"
#include "kroPipe_depth.hpp"


namespace KP {
namespace SWAPCHAIN {

struct SwapChainSupportDetails{
    
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain{
    
public:

uint32_t formatCount;
uint32_t presentModeCount;
bool swapChainAdequate = false;

inline SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) { // get information from our physical device and store it on "SwapChainSupportDetails details"
    
    SwapChainSupportDetails details;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }
    
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }
    
    return details;
}

inline VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}
inline VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}
inline VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(mWindow, &width, &height);
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }
}


inline void createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(g_PhysicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    KP::QUEUFAMILIES::QueueFamilyIndices indices = KP::QUEUFAMILIES::OBJECT_queuFamilies.findQueuFamilies(g_PhysicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    if(vsync){
        createInfo.presentMode = presentMode;
    }
    else{
        createInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    }
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    err = vkCreateSwapchainKHR(g_Device, &createInfo, Allocator, &swapChain);
    check_vk_result(err, "failed to create swap chain!");

    vkGetSwapchainImagesKHR(g_Device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    err = vkGetSwapchainImagesKHR(g_Device, swapChain, &imageCount, swapChainImages.data());
    check_vk_result(err);
    
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    std::vector<VkImage> swapChainImages;
    vkGetSwapchainImagesKHR(g_Device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(g_Device, swapChain, &imageCount, swapChainImages.data());

}

inline void cleanupSwapChain(){
    vkDestroyImageView(g_Device, depthImageView, Allocator);
    vkDestroyImage(g_Device, depthImage, Allocator);
    vkFreeMemory(g_Device, depthImageMemory, Allocator);

    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(g_Device, swapChainFramebuffers[i], Allocator);
    }

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        vkDestroyImageView(g_Device, swapChainImageViews[i], Allocator);
    }
    vkDestroySwapchainKHR(g_Device, swapChain, Allocator);
}

inline void recreateSwapChain(){
    
    int width = 0, height = 0;
    glfwGetFramebufferSize(mWindow, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(mWindow, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(g_Device);

    vkDeviceWaitIdle(g_Device);

    cleanupSwapChain();

    createSwapChain();
    KP::IMAGEVIEW::OBJECT_imageView.createImageViews();
    KP::DEPTH::OBJECT_depth.createDepthResources();
    KP::FRAMEBUFFER::OBJECT_frameBuffer.createFrameBuffers();

}    

};//CLASS SWAPCHAIN

inline KP::SWAPCHAIN::SwapChain OBJECT_swapChain;

}//SWAPCHAIN
}//KP




#endif //SWAPCHAIN_H
