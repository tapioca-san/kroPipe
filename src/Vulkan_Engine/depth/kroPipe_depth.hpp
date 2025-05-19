#ifndef KROPIPE_DEPTH_HPP
#define KROPIPE_DEPTH_HPP

#include "../../kroPipe_depedence.hpp"
#include "../device/kroPipe_device.hpp" // OBJECT_device
#include "../debug/kroPipe_debug.hpp"   // fatalMessage, check_vk_result, err, VK_Allocator
// Incluir header para createImage, createImageView se estiverem em outro lugar (ex: Texture)
// #include "../texture/kroPipe_texture.hpp"

#include <vector> // std::vector

namespace KP {
namespace ENGINE {

// Variáveis globais/namespace (declaradas extern)
extern VkImage depthImage; // Assinando como extern
extern VkDeviceMemory depthImageMemory; // Assinando como extern
extern VkImageView depthImageView; // Assinando como extern


class Depth{

public:
    // Métodos da classe Depth
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat findDepthFormat() {
        return findSupportedFormat(
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    bool hasStencilComponent(VkFormat format);

    void createDepthResources();

}; // CLASS DEPTH

// Objeto Depth (declarado extern)
extern KP::ENGINE::Depth OBJECT_depth;

} // namespace DEPTH
} // namespace KP

#endif//KROPIPE_DEPTH_HPP