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

    // Estas chamadas KP::TEXTURE::createImage e KP::TEXTURE::createImageView
    // dependem de onde você definiu essas funções. Elas precisam ser acessíveis.
    // Inclua o header ou o cpp onde elas estão e use o namespace/classe correto.
    // Exemplo assumindo que existem e são acessíveis:

    // Using swapChainExtent do namespace
    // KP::TEXTURE::createImage(KP::ENGINE::swapChainExtent.width, KP::ENGINE::swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    // depthImageView = KP::TEXTURE::createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    // Placeholder: Você precisa implementar a criação real da imagem e view de profundidade aqui
    // Usando depthImage, depthImageMemory, depthImageView do namespace para armazenar os resultados
    // Usando OBJECT_device, VK_Allocator, check_vk_result, err do namespace
    // Usando swapChainExtent do namespace

    // Exemplo básico de criação da imagem e alocação de memória (requer mais funções utilitárias)
    /*
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = KP::ENGINE::swapChainExtent.width;
    imageInfo.extent.height = KP::ENGINE::swapChainExtent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = depthFormat;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    KP::ENGINE::err = vkCreateImage(KP::ENGINE::OBJECT_device.VK_Device, &imageInfo, KP::ENGINE::VK_Allocator, &KP::ENGINE::depthImage);
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "Failed to create depth image!");

    // Você precisará de uma função para encontrar memória apropriada e alocar
    // VkMemoryRequirements memRequirements;
    // vkGetImageMemoryRequirements(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::depthImage, &memRequirements);
    // VkMemoryAllocateInfo allocInfo{};
    // allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    // allocInfo.allocationSize = memRequirements.size;
    // allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT); // findMemoryType é outra utilitária

    // KP::ENGINE::err = vkAllocateMemory(KP::ENGINE::OBJECT_device.VK_Device, &allocInfo, KP::ENGINE::VK_Allocator, &KP::ENGINE::depthImageMemory);
    // KP::ENGINE::check_vk_result(KP::ENGINE::err, "Failed to allocate depth image memory!");
    // vkBindImageMemory(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::depthImage, KP::ENGINE::depthImageMemory, 0);

    // Exemplo básico de criação do ImageView de profundidade
    // VkImageViewCreateInfo viewInfo{};
    // viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    // viewInfo.image = KP::ENGINE::depthImage;
    // viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    // viewInfo.format = depthFormat;
    // viewInfo.subresourceRange.baseMipLevel = 0;
    // viewInfo.subresourceRange.levelCount = 1;
    // viewInfo.subresourceRange.baseArrayLayer = 0;
    // viewInfo.subresourceRange.layerCount = 1;
    // viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT; // Pode precisar VK_IMAGE_ASPECT_STENCIL_BIT também se format tiver stencil

    // if (hasStencilComponent(depthFormat)) { // Chamando método da classe
    //     viewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    // }

    // KP::ENGINE::err = vkCreateImageView(KP::ENGINE::OBJECT_device.VK_Device, &viewInfo, KP::ENGINE::VK_Allocator, &KP::ENGINE::depthImageView);
    // KP::ENGINE::check_vk_result(KP::ENGINE::err, "Failed to create depth image view!");
    */
}

} // namespace ENGINE
} // namespace KP