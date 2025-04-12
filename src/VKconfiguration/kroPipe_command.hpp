#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include "../VKkropipe/kroPipe_struct.hpp"
#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_extension.hpp"
#include "kroPipe_queuFamilies.hpp"
#include "kroPipe_vertex.hpp"

void createCommandPool(){

    QueueFamilyIndices queueFamilyIndices = findQueuFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error(fatalMensage("failed to create command pool!"));
    }
    
}

void createCommandBuffers() { // já fiz -> fazer uam struct que coloque isso como parametro nessa função e use nvim. faça que o proprio model renderize isso na tela só com o construtor
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error(fatalMensage("failed to allocate command buffers!"));
    }
}





void destroyCommandPool(){
    vkDestroyCommandPool(device, commandPool, nullptr);

}
#endif //COMMANDBUFFER_H