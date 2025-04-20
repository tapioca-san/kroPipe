#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"

namespace KP {
namespace FRAMEBUFFER {


inline void createFrameBuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::array<VkImageView, 2> attachments = {
            swapChainImageViews[i],
            depthImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, Allocator, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error(fatalMensage("failed to create framebuffer!"));
        }
    }
}



inline void CleanUpFramerBuffer(){
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, Allocator);
    }
}


}//FRAMEBUFFER
}//KP
#endif //FRAMEBUFFER_H