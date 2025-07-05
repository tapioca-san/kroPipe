#include "../swapchain/kroPipe_swapchain.hpp"
#include "../depth/kroPipe_depth.hpp"
#include "kroPipe_texture.hpp"
#include <vulkan/vulkan_core.h>
#include "kroPipe_MSAA.hpp"


namespace KP {
namespace ENGINE {

VkDeviceMemory* ImageMSAA::getPointerColorImageMemory(){
    return &colorImageMemory;
}
VkImage* ImageMSAA::getPointerColorImage(){
    return &colorImage;
}
VkImageView* ImageMSAA::colorPointerImageColorView(){
    return &colorImageView;
}

ImageMSAA* MSAA::getPointerDataImage(){
    return &dataImage;
}

VkSampleCountFlagBits MSAA::getMaxUsableSampleCount() {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(KP::ENGINE::OBJECT_device.getPhysicalDevice(), &physicalDeviceProperties);
        
    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
    
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return (VkSampleCountFlagBits)counts;
}

void MSAA::createColorResources() {
        VkFormat colorFormat = swapChainImageFormat;

        KP::ENGINE::createImage(swapChainExtent.width, swapChainExtent.height, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *getPointerDataImage()->getPointerColorImage(), *getPointerDataImage()->getPointerColorImageMemory());
        
        KP::ENGINE::createImageView(*getPointerDataImage()->getPointerColorImage(), colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
       
}

void MSAA::clean(){

    vkDestroyImageView(KP::ENGINE::OBJECT_device.getDevice(), *getPointerDataImage()->colorPointerImageColorView(), KP::ENGINE::VK_Allocator);
    vkDestroyImage(KP::ENGINE::OBJECT_device.getDevice(), *getPointerDataImage()->getPointerColorImage(), KP::ENGINE::VK_Allocator);
    vkFreeMemory(KP::ENGINE::OBJECT_device.getDevice(), *getPointerDataImage()->getPointerColorImageMemory(), KP::ENGINE::VK_Allocator);
}
    

/*
void MSAA::createDepthResources() {
    VkFormat depthFormat = KP::ENGINE::findDepthFormat();
    createImage(swapChainExtent.width, swapChainExtent.height, 1, *OBJECT_msaa.getPointerMsaaSamples(), depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
}
*/

VkSampleCountFlagBits* MSAA::getPointerMsaaSamples(){
    return &msaaSamples;
}

KP::ENGINE::MSAA OBJECT_msaa;


}//ENGINE
}//KP