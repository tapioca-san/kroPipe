


#include "../swapchain/kroPipe_swapchain.hpp" 
#include "../texture/kroPipe_texture.hpp"


namespace KP {
namespace ENGINE {



// Definição dos métodos da classe ImageView

void ImageView::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        swapChainImageViews[i] = KP::ENGINE::createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
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