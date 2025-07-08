


#include "../swapchain/kroPipe_swapchain.hpp" 
#include "../texture/kroPipe_texture.hpp"


namespace KP {
namespace ENGINE {

void ImageView::createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        swapChainImageViews[i] = KP::ENGINE::createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

void ImageView::DestroyImageview(){
    for (auto imageView : KP::ENGINE::swapChainImageViews) {
        vkDestroyImageView(*KP::ENGINE::OBJECT_device.getPointerDevice(), imageView, KP::ENGINE::VK_Allocator);
    }
    KP::ENGINE::swapChainImageViews.clear();
}

KP::ENGINE::ImageView OBJECT_imageView;

} // namespace ENGINE
} // namespace KP