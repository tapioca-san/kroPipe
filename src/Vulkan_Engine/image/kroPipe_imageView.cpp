//#include "kroPipe_imageView.hpp"
//include "../device/kroPipe_device.hpp" // OBJECT_device
//#include "../debug/kroPipe_debug.hpp"   // VK_Allocator, check_vk_result, err

// includes em cima são desnecessario para esse caso. caso precise, remova


#include "../swapchain/kroPipe_swapchain.hpp" // swapChainImageFormat, swapChainImages (já declarados no .hpp deste arquivo)

//#include "../device/kroPipe_device.hpp" // OBJECT_device não necessario ao caso

namespace KP {
namespace ENGINE {

// Definições das variáveis globais/namespace (sem extern)
std::vector<VkImage> swapChainImages; // Definição
std::vector<VkImageView> swapChainImageViews; // Definição


// Definição dos métodos da classe ImageView

void ImageView::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size()); // Usando variáveis do namespace

    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        // Esta parte depende de onde a função createImageView está definida.
        // Se estiver em KroPipe::TEXTURE, você precisará incluir o header correto
        // e usar a chamada apropriada (e talvez a classe/objeto Textura).
        // Exemplo assumindo que existe uma função global/namespace ou membro estática:
        // swapChainImageViews[i] = KP::TEXTURE::createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);

        // Placeholder: Você precisa implementar a criação real do ImageView aqui
        // Exemplo Básico (pode precisar ajustar dependendo da sua implementação real de createImageView):
         VkImageViewCreateInfo viewInfo{};
         viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
         viewInfo.image = swapChainImages[i];
         viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
         viewInfo.format = swapChainImageFormat;
         viewInfo.subresourceRange.baseMipLevel = 0;
         viewInfo.subresourceRange.levelCount = 1;
         viewInfo.subresourceRange.baseArrayLayer = 0;
         viewInfo.subresourceRange.layerCount = 1;
         viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

         // Usando OBJECT_device, VK_Allocator, check_vk_result, err do namespace
         KP::ENGINE::err = vkCreateImageView(KP::ENGINE::OBJECT_device.VK_Device, &viewInfo, KP::ENGINE::VK_Allocator, &swapChainImageViews[i]);
         KP::ENGINE::check_vk_result(KP::ENGINE::err, "Failed to create image view!"); // Adicionado mensagem de erro
    }
}

void ImageView::DestroyImageview(){
    // Usando swapChainImageViews e OBJECT_device, VK_Allocator do namespace
    for (auto imageView : KP::ENGINE::swapChainImageViews) {
        vkDestroyImageView(KP::ENGINE::OBJECT_device.VK_Device, imageView, KP::ENGINE::VK_Allocator);
    }
    KP::ENGINE::swapChainImageViews.clear(); // Limpar o vector após destruir
}

// Definição do objeto ImageView (sem extern)
KP::ENGINE::ImageView OBJECT_imageView;

} // namespace ENGINE
} // namespace KP