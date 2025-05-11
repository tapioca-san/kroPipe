

#ifndef IMAGEVIEW_H 
#define IMAGEVIEW_H

#include "../VKkropipe/kroPipe_texture.hpp"
#include "../kroPipe_include.hpp"

namespace KP {
namespace IMAGEVIEW {

class ImageView{
    
public:

inline void createImageViews() {
    swapChainImageViews.resize(swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        swapChainImageViews[i] = KP::TEXTURE::createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

inline void DestroyImageview(){
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(g_Device, imageView, Allocator);
    }
}
};//CLASS IMAGEVIEW
inline KP::IMAGEVIEW::ImageView OBJECT_imageView;
}//IMAGEVIEW
}//KP
#endif //IMAGEVIEW_H