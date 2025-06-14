#include "kroPipe_command.hpp"

#include "../queue/kroPipe_queuFamilies.hpp"
#include "../device/kroPipe_device.hpp"
#include "../render/kroPipe_render.hpp"
#include "../debug/kroPipe_debug.hpp"


 void KP::ENGINE::Command::createCommandPool(){
    KP::ENGINE::QueueFamilyIndices queueFamilyIndices = KP::ENGINE::OBJECT_queuFamilies.findQueuFamilies(KP::ENGINE::OBJECT_device.getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    err = vkCreateCommandPool(KP::ENGINE::OBJECT_device.getDevice(), &poolInfo, KP::ENGINE::VK_Allocator, &commandPool);
    check_vk_result(err, "failed to create command pool!");
    
}

 void KP::ENGINE::Command::createCommandBuffers() { 
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    err = vkAllocateCommandBuffers(KP::ENGINE::OBJECT_device.getDevice(), &allocInfo, commandBuffers.data());
    check_vk_result(err, "failed to allocate command buffers!");
    
}

 void KP::ENGINE::Command::createDescriptorPool(VkDescriptorPool &descriptorPool) {
    
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

    err = vkCreateDescriptorPool(KP::ENGINE::OBJECT_device.getDevice(), &poolInfo, KP::ENGINE::VK_Allocator, &descriptorPool);
    check_vk_result(err, "failed to create descriptor pool!");
}

 void KP::ENGINE::Command::destroyCommandPool(){
    vkDestroyCommandPool(KP::ENGINE::OBJECT_device.getDevice(), commandPool, nullptr);

}

namespace KP {
namespace ENGINE {

std::vector<VkCommandBuffer> commandBuffers;// crialão da variavel que foi declarada no kroPipe_command.hpp

VkCommandPool commandPool;// criação da variavel que foi declarada no kroPipe_command.hpp

KP::ENGINE::Command OBJECT_command; // criação da variavel já declarada no kroPipe_command.hpp com extern

}//ENGINE
}//KP