#include "kroPipe_frameBuffer.hpp"
#include "../swapchain/kroPipe_swapchain.hpp" // Necessário para swapChainImageViews, swapChainExtent, swapChainFramebuffers
#include "../pipeline/kroPipe_pipeline.hpp" // Necessário para VK_renderPass
// Incluído no .hpp agora que as definições estão separadas
// #include "../swapchain/kroPipe_swapchain.hpp"
// #include "../pipeline/kroPipe_pipeline.hpp"
// #include "../image/kroPipe_imageView.hpp"
// #include "../device/kroPipe_device.hpp"
// #include "../debug/kroPipe_debug.hpp"
// #include "../depth/kroPipe_depth.hpp"

namespace KP {
namespace ENGINE {

// Definição do objeto FrameBuffer (sem extern)
KP::ENGINE::FrameBuffer OBJECT_frameBuffer;

// Definição dos métodos da classe FrameBuffer

void FrameBuffer::createFrameBuffers() {
    // Usando variáveis/objetos do namespace KP::ENGINE
    KP::ENGINE::swapChainFramebuffers.resize(KP::ENGINE::swapChainImageViews.size());

    for (size_t i = 0; i < KP::ENGINE::swapChainImageViews.size(); i++) {
        std::array<VkImageView, 2> attachments = {
            KP::ENGINE::swapChainImageViews[i], // Usando swapChainImageViews do namespace
            KP::ENGINE::depthImageView          // Usando depthImageView do namespace
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = KP::ENGINE::VK_renderPass; // Usando VK_renderPass do namespace
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = KP::ENGINE::swapChainExtent.width; // Usando swapChainExtent do namespace
        framebufferInfo.height = KP::ENGINE::swapChainExtent.height; // Usando swapChainExtent do namespace
        framebufferInfo.layers = 1;

        // Usando OBJECT_device, VK_Allocator, fatalMessage do namespace
        if (vkCreateFramebuffer(KP::ENGINE::OBJECT_device.VK_Device, &framebufferInfo, KP::ENGINE::VK_Allocator, &KP::ENGINE::swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error(KP::ENGINE::fatalMessage("failed to create framebuffer!"));
        }
    }
}

void FrameBuffer::CleanUpFramerBuffer(){
    // Usando swapChainFramebuffers, OBJECT_device, VK_Allocator do namespace
    for (auto framebuffer : KP::ENGINE::swapChainFramebuffers) {
        vkDestroyFramebuffer(KP::ENGINE::OBJECT_device.VK_Device, framebuffer, KP::ENGINE::VK_Allocator);
    }
    KP::ENGINE::swapChainFramebuffers.clear(); // Limpar o vector após destruir
}

} // namespace ENGINE
} // namespace KP