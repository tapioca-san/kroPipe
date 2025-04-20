#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include "../VKkropipe/kroPipe_struct.hpp"
#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_extension.hpp"
#include "kroPipe_queuFamilies.hpp"
#include "kroPipe_vertex.hpp"

namespace KP {
namespace COMMANDBUFFER {


void createCommandPool(){

    QueueFamilyIndices queueFamilyIndices = findQueuFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    err = vkCreateCommandPool(device, &poolInfo, Allocator, &commandPool);
    check_vk_result(err, "failed to create command pool!");
    
}

void createCommandBuffers() { 
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    err = vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());
    check_vk_result(err, "failed to allocate command buffers!");
    
}

void createDescriptorPool(VkDescriptorPool &descriptorPool) {
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    err = vkCreateDescriptorPool(device, &poolInfo, Allocator, &descriptorPool);
    check_vk_result(err, "failed to create descriptor pool!");
}

void destroyCommandPool(){
    vkDestroyCommandPool(device, commandPool, nullptr);

}
}//COMMANDBUFFER
}//KP

#endif //COMMANDBUFFER_H