#include "../swapchain/kroPipe_swapchain.hpp"
#include "../depth/kroPipe_depth.hpp"
#include "kroPipe_texture.hpp"
#include "kroPipe_MSAA.hpp"


namespace KP {
namespace ENGINE {

VkDeviceMemory* ImageMSAA::getPointerColorDeviceMemory(){
    return &colorDeviceMemory;
}
VkImage* ImageMSAA::getPointerColorImage(){
    return &colorImage;
}
VkImageView* ImageMSAA::getPointerImageColorView(){
    return &colorImageColorView;
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
    
    KP::ENGINE::createImage(swapChainExtent.width, swapChainExtent.height, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *getPointerDataImage()->getPointerColorImage(), *getPointerDataImage()->getPointerColorDeviceMemory());
    
    *dataImage.getPointerImageColorView() = KP::ENGINE::createImageView(*getPointerDataImage()->getPointerColorImage(), colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    KP::ENGINE::OBJECT_debugger.setDebugName(KP::ENGINE::OBJECT_device.getDevice(), (uint64_t)(*dataImage.getPointerColorDeviceMemory()), VK_OBJECT_TYPE_DEVICE_MEMORY, "anjinho");
    
    if(*getAbleMsaa() == true){
        OBJECT_msaa.setPointMsaaSamples(OBJECT_msaa.getMaxUsableSampleCount());
    }
    else {
        OBJECT_msaa.setPointMsaaSamples(VK_SAMPLE_COUNT_1_BIT);
    }
}

void MSAA::setPointMsaaSamples(VkSampleCountFlagBits MsaaSample){
    this->msaaSamples = MsaaSample;
}

bool* MSAA::getAbleMsaa(){
    return &enableMsaa;
}

void MSAA::clean(){

    vkDestroyImageView(KP::ENGINE::OBJECT_device.getDevice(), *getPointerDataImage()->getPointerImageColorView(), KP::ENGINE::VK_Allocator);
    vkDestroyImage(KP::ENGINE::OBJECT_device.getDevice(), *getPointerDataImage()->getPointerColorImage(), KP::ENGINE::VK_Allocator);
    vkFreeMemory(KP::ENGINE::OBJECT_device.getDevice(), *getPointerDataImage()->getPointerColorDeviceMemory(), KP::ENGINE::VK_Allocator);
}

VkSampleCountFlagBits* MSAA::getPointerMsaaSamples(){
    return &msaaSamples;
}


KP::ENGINE::MSAA OBJECT_msaa;


}//ENGINE
}//KP