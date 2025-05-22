#include "../queue/kroPipe_queuFamilies.hpp"
#include "../command/kroPipe_command.hpp"
#include "../device/kroPipe_device.hpp"
#include "../debug/kroPipe_debug.hpp"
#include "kroPipe_vertex.hpp"

uint32_t KP::ENGINE::Vertex::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    
    vkGetPhysicalDeviceMemoryProperties(KP::ENGINE::OBJECT_device.VK_PhysicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error(fatalMessage("failed to find suitable memory type!"));
    return 0;
}

VkCommandBuffer KP::ENGINE::Vertex::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = KP::ENGINE::commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    err = vkAllocateCommandBuffers(KP::ENGINE::OBJECT_device.VK_Device, &allocInfo, &commandBuffer);
    check_vk_result(err);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    err = vkBeginCommandBuffer(commandBuffer, &beginInfo);
    check_vk_result(err);

    return commandBuffer;
}

 void KP::ENGINE::Vertex::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    err = vkEndCommandBuffer(commandBuffer);
    check_vk_result(err);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    err = vkQueueSubmit(KP::ENGINE::OBJECT_queuFamilies.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    check_vk_result(err);
    err = vkQueueWaitIdle(KP::ENGINE::OBJECT_queuFamilies.graphicsQueue);
    check_vk_result(err);
    
    vkFreeCommandBuffers(KP::ENGINE::OBJECT_device.VK_Device, commandPool, 1, &commandBuffer);
}

namespace KP {
namespace ENGINE {

KP::ENGINE::Vertex OBJECT_vertex;
VkMemoryRequirements memRequirements;
VkPhysicalDeviceMemoryProperties memProperties;

}//VERTEX
}//KP
