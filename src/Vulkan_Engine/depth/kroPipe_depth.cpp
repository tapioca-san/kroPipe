#include "../swapchain/kroPipe_swapchain.hpp" 
#include "../texture/kroPipe_texture.hpp"
#include "../device/kroPipe_device.hpp" 
#include "../texture/kroPipe_MSAA.hpp"
#include "../debug/kroPipe_debug.hpp" 
#include "kroPipe_depth.hpp"

namespace KP {
namespace ENGINE {

VkImage depthImage = VK_NULL_HANDLE; 
VkDeviceMemory depthImageMemory = VK_NULL_HANDLE; 
VkImageView depthImageView = VK_NULL_HANDLE; 

KP::ENGINE::Depth OBJECT_depth;


VkFormat Depth::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(KP::ENGINE::OBJECT_device.getPhysicalDevice(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error(fatalMessage("failed to find supported format!"));
}


bool Depth::hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat Depth::findDepthFormat() {
    return findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

void Depth::createDepthResources(){
        
        VkFormat depthFormat = findDepthFormat();
        KP::ENGINE::createImage(swapChainExtent.width, swapChainExtent.height, *KP::ENGINE::OBJECT_msaa.getPointerMsaaSamples(),depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
        depthImageView = KP::ENGINE::createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
        
    }

} // namespace ENGINE
} // namespace KP