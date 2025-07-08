#include "kroPipe_frameBuffer.hpp"
#include "../swapchain/kroPipe_swapchain.hpp" 
#include "../pipeline/kroPipe_pipeline.hpp" 
#include "../texture/kroPipe_MSAA.hpp"
namespace KP {
namespace ENGINE {

KP::ENGINE::FrameBuffer OBJECT_frameBuffer;


void FrameBuffer::createFrameBuffers() {
    swapChainFramebuffers.resize(swapChainImageViews.size());
    
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    std::vector<VkImageView> attachments;
    if(*KP::ENGINE::OBJECT_msaa.getAbleMsaa() == true){
        attachments = {
            *KP::ENGINE::OBJECT_msaa.getPointerDataImage()->getPointerImageColorView(),
            depthImageView,
            swapChainImageViews[i],
        };
    }
    else{
        attachments = {
            swapChainImageViews[i],
            depthImageView,
        };
    }
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = KP::ENGINE::VK_renderPass;;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(*KP::ENGINE::OBJECT_device.getPointerDevice(), &framebufferInfo, KP::ENGINE::VK_Allocator, &KP::ENGINE::swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error(fatalMessage("failed to create framebuffer!"));
        }
    }
}

void FrameBuffer::CleanUpFramerBuffer(){
    for (auto framebuffer : KP::ENGINE::swapChainFramebuffers) {
        vkDestroyFramebuffer(*KP::ENGINE::OBJECT_device.getPointerDevice(), framebuffer, KP::ENGINE::VK_Allocator);
    }
    KP::ENGINE::swapChainFramebuffers.clear(); 
}

} // namespace ENGINE
} // namespace KP