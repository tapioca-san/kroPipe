#include "../window/kroPipe_windowSurface.hpp"
#include "../buffers/kroPipe_frameBuffer.hpp" 
#include "../queue/kroPipe_queuFamilies.hpp" 
#include "../image/kroPipe_imageView.hpp"   
#include "../window/kroPipe_window.hpp"
#include "../render/kroPipe_render.hpp"
#include "../device/kroPipe_device.hpp"
#include "../texture/kroPipe_MSAA.hpp"
#include "../depth/kroPipe_depth.hpp"      
#include "../debug/kroPipe_debug.hpp" 
#include "kroPipe_swapchain.hpp"

namespace KP {
namespace ENGINE {

std::vector<VkImage> swapChainImages; 
std::vector<VkImageView> swapChainImageViews; 
VkFormat swapChainImageFormat;
VkExtent2D swapChainExtent;
VkSwapchainKHR swapChain = VK_NULL_HANDLE; 
std::vector<VkFramebuffer> swapChainFramebuffers; 
bool vsync = true; 

KP::ENGINE::SwapChain OBJECT_swapChain;

SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    check_vk_result(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, KP::ENGINE::OBJECT_windowSurface.VK_surface, &details.capabilities));

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, KP::ENGINE::OBJECT_windowSurface.VK_surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, KP::ENGINE::OBJECT_windowSurface.VK_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, KP::ENGINE::OBJECT_windowSurface.VK_surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, KP::ENGINE::OBJECT_windowSurface.VK_surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(KP::ENGINE::OBJECT_window.GLFW_window, &width, &height);
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }
}

void SwapChain::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(KP::ENGINE::OBJECT_device.getPhysicalDevice());

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats); 
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes); 
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities); 

    uint32_t imageCount = MAX_FRAMES_IN_FLIGHT;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = KP::ENGINE::OBJECT_windowSurface.VK_surface; 

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    KP::ENGINE::QueueFamilyIndices indices = KP::ENGINE::OBJECT_queuFamilies.findQueuFamilies(KP::ENGINE::OBJECT_device.getPhysicalDevice());
    uint32_t queueFamilyIndices[] = {indices.graphicsAndComputeFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsAndComputeFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    if(KP::ENGINE::vsync){
        createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // VSYNC
    }
    else{
        createInfo.presentMode = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR; // NO VSYNC
    }
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE; 

    KP::ENGINE::err = vkCreateSwapchainKHR(KP::ENGINE::OBJECT_device.getDevice(), &createInfo, KP::ENGINE::VK_Allocator, &swapChain); // Cria o membro swapChain
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "failed to create swap chain!");

    vkGetSwapchainImagesKHR(KP::ENGINE::OBJECT_device.getDevice(), swapChain, &imageCount, nullptr);
    KP::ENGINE::swapChainImages.resize(imageCount);
    KP::ENGINE::err = vkGetSwapchainImagesKHR(KP::ENGINE::OBJECT_device.getDevice(), swapChain, &imageCount, KP::ENGINE::swapChainImages.data());
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "Failed to get swap chain images"); 
    KP::ENGINE::swapChainImageFormat = surfaceFormat.format;
    KP::ENGINE::swapChainExtent = extent;

    
    std::vector<VkImage> swapChainImages;
    vkGetSwapchainImagesKHR(KP::ENGINE::OBJECT_device.getDevice(), swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(KP::ENGINE::OBJECT_device.getDevice(), swapChain, &imageCount, swapChainImages.data());
    
}

void SwapChain::cleanupSwapChain(){
    KP::ENGINE::OBJECT_msaa.clean();
    vkDestroyImageView(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::depthImageView, KP::ENGINE::VK_Allocator);
    vkDestroyImage(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::depthImage, KP::ENGINE::VK_Allocator);
    vkFreeMemory(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::depthImageMemory, KP::ENGINE::VK_Allocator); 

    for (auto framebuffer : KP::ENGINE::swapChainFramebuffers) {
        vkDestroyFramebuffer(KP::ENGINE::OBJECT_device.getDevice(), framebuffer, KP::ENGINE::VK_Allocator);
    }

    for (auto imageView : KP::ENGINE::swapChainImageViews) {
        vkDestroyImageView(KP::ENGINE::OBJECT_device.getDevice(), imageView, KP::ENGINE::VK_Allocator);
    }

    vkDestroySwapchainKHR(KP::ENGINE::OBJECT_device.getDevice(), swapChain, KP::ENGINE::VK_Allocator);
}

void SwapChain::recreateSwapChain(){
    int width = 0, height = 0;
    glfwGetFramebufferSize(KP::ENGINE::OBJECT_window.GLFW_window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(KP::ENGINE::OBJECT_window.GLFW_window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(KP::ENGINE::OBJECT_device.getDevice());

    cleanupSwapChain(); 

    createSwapChain();
    KP::ENGINE::OBJECT_imageView.createImageViews();
    KP::ENGINE::OBJECT_msaa.createColorResources();
    KP::ENGINE::OBJECT_depth.createDepthResources();
    KP::ENGINE::OBJECT_frameBuffer.createFrameBuffers();
}

} // namespace ENGINE
} // namespace KP