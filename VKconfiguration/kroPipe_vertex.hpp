#ifndef VERTEX_H
#define VERTEX_H

//#include "../VKkropipe/kroPipe_model.hpp"

#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"

namespace KP {
namespace VERTEX {

class Vertex{
public:

inline uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    
    vkGetPhysicalDeviceMemoryProperties(g_PhysicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error(fatalMensage("failed to find suitable memory type!"));
    return 0;
}

inline VkCommandBuffer beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    err = vkAllocateCommandBuffers(g_Device, &allocInfo, &commandBuffer);
    check_vk_result(err);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    err = vkBeginCommandBuffer(commandBuffer, &beginInfo);
    check_vk_result(err);

    return commandBuffer;
}

inline void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    err = vkEndCommandBuffer(commandBuffer);
    check_vk_result(err);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    err = vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    check_vk_result(err);
    err = vkQueueWaitIdle(graphicsQueue);
    check_vk_result(err);
    
    vkFreeCommandBuffers(g_Device, commandPool, 1, &commandBuffer);
}

};//CLASS VERTEX

inline KP::VERTEX::Vertex OBJECT_vertex;
}//VERTEX
}//KP


#endif //VERTEX_H