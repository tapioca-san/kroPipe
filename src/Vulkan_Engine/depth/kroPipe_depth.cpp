#include "kroPipe_depth.hpp"
#include "../device/kroPipe_device.hpp" // OBJECT_device
#include "../debug/kroPipe_debug.hpp"   // fatalMessage, check_vk_result, err, VK_Allocator
#include "../swapchain/kroPipe_swapchain.hpp" // swapChainExtent (já declarados no .hpp deste arquivo)
// Incluir .cpp ou .hpp para createImage, createImageView se estiverem em outro lugar
// #include "../texture/kroPipe_texture.cpp" // Se as definições estiverem em um .cpp

namespace KP {
namespace ENGINE {

// Definições das variáveis globais/namespace (sem extern)
VkImage depthImage = VK_NULL_HANDLE; // Definição
VkDeviceMemory depthImageMemory = VK_NULL_HANDLE; // Definição
VkImageView depthImageView = VK_NULL_HANDLE; // Definição

// Definição do objeto Depth (sem extern)
KP::ENGINE::Depth OBJECT_depth;

// Definição dos métodos da classe Depth

VkFormat Depth::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    // Usando OBJECT_device do namespace e fatalMessage do namespace
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(KP::ENGINE::OBJECT_device.VK_PhysicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error(KP::ENGINE::fatalMessage("failed to find supported format!"));
}


bool Depth::hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}


void Depth::createDepthResources(){
    VkFormat depthFormat = findDepthFormat(); // Chamando método da classe

   
}

} // namespace ENGINE
} // namespace KP