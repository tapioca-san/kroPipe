#include "../../Utils/object/kroPipe_object.hpp"
#include "../../Utils/imgui/kroPipe_imgui.hpp"
#include "../../Utils/input/kroPipe_input.hpp"
#include "../swapchain/kroPipe_swapchain.hpp"
#include "../pipeline/kroPipe_pipeline.hpp"
#include "../OS/kroPipe_os.hpp"
#include "kroPipe_render.hpp"

namespace KP {
namespace ENGINE {

// max frames reclama quando está mais de 2 quando estiver no hyprland. no X11, o MAX_FRAMES_IN_FLIGHT maior que 2 funciona perfeitamente
int MAX_FRAMES_IN_FLIGHT; // quando crescer, adiiconar 3 aqui e gerenciar isso.

Render::Render(){
    
    if (KP::ENGINE::Object_operationSystem.isWayland()) {
        MAX_FRAMES_IN_FLIGHT = 2;
    }
    
    if (KP::ENGINE::Object_operationSystem.isX11()) {
        MAX_FRAMES_IN_FLIGHT = 3;
    }
}

std::vector<VkSemaphore> imageAvailableSemaphores;
std::vector<VkSemaphore> renderFinishedSemaphores;
std::vector<VkFence> inFlightFences;
uint32_t imageIndex;
bool framebufferResized = false;

KP::ENGINE::Render OBJECT_render;


VkFenceCreateInfo Render::FenceCreateInfo(VkFenceCreateFlags flags){
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = flags;
    return fenceCreateInfo;
}

VkSemaphoreCreateInfo Render::SemaphoreCreateInfo(VkSemaphoreCreateFlags flags){
    VkSemaphoreCreateInfo semCreateInfo = {};
    semCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semCreateInfo.pNext = nullptr;
    semCreateInfo.flags = flags;
    return semCreateInfo;
}


void KP::ENGINE::Render::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error(fatalMessage("failed to begin recording command buffer!"));
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = VK_renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{1.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};
    
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, KP::ENGINE::graphicsPipeline);
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;

    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    for(KP::UTILS::Object* obj : *KP::UTILS::OBJECT_objectsManager.getAllObject()){
        obj->getData().model->draw(commandBuffer);
    }

    //IMGUI RENDER 
    if(KP::UTILS::ImguiMode){

        KP::UTILS::OBJECT_imguiInterface->render(commandBuffer);
    }


    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error(fatalMessage("failed to record command buffer!"));
    }
}

VkResult KP::ENGINE::Render::acquireNextImage(){
    VkResult result = vkAcquireNextImageKHR(*KP::ENGINE::OBJECT_device.getPointerDevice(), swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        KP::ENGINE::OBJECT_swapChain.recreateSwapChain();
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error(fatalMessage("failed to acquire swap chain image!"));
    }
    return result;
}

void KP::ENGINE::Render::drawFrame() {
    vkWaitForFences(*KP::ENGINE::OBJECT_device.getPointerDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    err = acquireNextImage();
    check_vk_result(err);

    for(uint16_t i = 0; i < KP::UTILS::OBJECT_objectsManager.getAllObject()->size(); i++){
        KP::UTILS::OBJECT_objectsManager.getModelByID(i)->UBO.update();
    }
    /*

    for(KP::UTILS::Model* model : KP::UTILS::OBJECT_objectsManager.getAllModel()){
        //model->UBO.update();    
    }  
    */
    
    vkResetFences(*KP::ENGINE::OBJECT_device.getPointerDevice(), 1, &inFlightFences[currentFrame]);

    vkResetCommandBuffer(KP::ENGINE::commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
   
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);
    
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1; // attention
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(KP::ENGINE::OBJECT_queuFamilies.graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error(fatalMessage("failed to submit draw command buffer!"));
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    err = vkQueuePresentKHR(KP::ENGINE::OBJECT_queuFamilies.presentQueue, &presentInfo);
    
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        KP::ENGINE::OBJECT_swapChain.recreateSwapChain();
    } else if (err != VK_SUCCESS) {
        throw std::runtime_error(fatalMessage("failed to present swap chain image!"));
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void KP::ENGINE::Render::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = SemaphoreCreateInfo(0);  
    VkFenceCreateInfo fenceInfo = FenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);


    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(*KP::ENGINE::OBJECT_device.getPointerDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(*KP::ENGINE::OBJECT_device.getPointerDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(*KP::ENGINE::OBJECT_device.getPointerDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error(fatalMessage("failed to create synchronization objects for a frame!"));
        }
    }
}

void KP::ENGINE::Render::destroyRender(){

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(*KP::ENGINE::OBJECT_device.getPointerDevice(), renderFinishedSemaphores[i], KP::ENGINE::VK_Allocator);
        vkDestroySemaphore(*KP::ENGINE::OBJECT_device.getPointerDevice(), imageAvailableSemaphores[i], KP::ENGINE::VK_Allocator);
        vkDestroyFence(*KP::ENGINE::OBJECT_device.getPointerDevice(), inFlightFences[i], KP::ENGINE::VK_Allocator);
    }
}

}//ENGINE
}//KP
