

#ifndef IMAGEVIEW_H 
#define IMAGEVIEW_H

#include "../VKkropipe/kroPipe_texture.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_extension.hpp"

void createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

void DestroyImageview(){
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }
}

#endif //IMAGEVIEW_H