#include "../../Utils/object/kroPipe_object.hpp"
#include "../../Utils/object/kroPipe_light.hpp"
#include "kroPipe_bufferManagerMacro.hpp"
#include "../device/kroPipe_device.hpp"
#include "../vertex/kroPipe_vertex.hpp"
#include "../debug/kroPipe_debug.hpp"
#include <string>
#include "kroPipe_bufferManager.hpp"

namespace KP {
namespace ENGINE {

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                                 VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult err = vkCreateBuffer(*OBJECT_device.getPointerDevice(), &bufferInfo, nullptr, &buffer);
    check_vk_result(err, "failed to create buffer!");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(*OBJECT_device.getPointerDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = OBJECT_vertex.findMemoryType(memRequirements.memoryTypeBits, properties);

    err = vkAllocateMemory(*OBJECT_device.getPointerDevice(), &allocInfo, VK_Allocator, &bufferMemory);
    check_vk_result(err, "failed to allocate buffer memory!");

    err = vkBindBufferMemory(*OBJECT_device.getPointerDevice(), buffer, bufferMemory, 0);
    check_vk_result(err);
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = OBJECT_vertex.beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    OBJECT_vertex.endSingleTimeCommands(commandBuffer);
}

void BufferManager::storeBuffer(std::shared_ptr<VkBuffer> buffer,
                                std::shared_ptr<VkDeviceMemory> buffersMemory,
                                void* bufferMapped) {
    buffers.uniformBuffers.push_back(*buffer);
    buffers.uniformBuffersMemory.push_back(*buffersMemory);
    buffers.uniformBuffersMapped.push_back(bufferMapped);
}

template<class t>
uint32_t BufferManager::addBuffer(t classType) {
    std::shared_ptr<VkBuffer> buffer = std::make_shared<VkBuffer>();
    std::shared_ptr<VkDeviceMemory> bufferMemory = std::make_shared<VkDeviceMemory>();
    void* bufferMapped;

    VkDeviceSize bufferSize = sizeof(t);
    createBuffer(bufferSize,
                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 *buffer, *bufferMemory);

    VkResult err = vkMapMemory(*OBJECT_device.getPointerDevice(), *bufferMemory, 0, bufferSize, 0, &bufferMapped);
    check_vk_result(err);

    storeBuffer(buffer, bufferMemory, bufferMapped);

    return buffers.lastID++;
}

template<class t>
void BufferManager::sendBufferDataToGpu(uint32_t bufferID, t data) {
    if (bufferID >= buffers.uniformBuffersMapped.size()) {
        throw std::runtime_error("buffer ID is not matching with the lists Buffers. ID you tried: " + std::to_string(bufferID));
    }

    memcpy(buffers.uniformBuffersMapped[bufferID], &data, sizeof(t));
}

void BufferManager::cleanUp(){
    for(uint32_t i = 0; i < buffers.lastID; i++){
        vkDestroyBuffer(*KP::ENGINE::OBJECT_device.getPointerDevice(), buffers.uniformBuffers[i], KP::ENGINE::VK_Allocator);
        vkFreeMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(), buffers.uniformBuffersMemory[i], KP::ENGINE::VK_Allocator);
    }
}

VkBuffer BufferManager::getUniformBufferByID(uint32_t bufferID){
    return buffers.uniformBuffers[bufferID];
}

VkDeviceMemory BufferManager::getUniformBufferMemoryByID(uint32_t bufferID){
    return buffers.uniformBuffersMemory[bufferID];
}

BufferManager OBJECT_bufferManager;


INSTANTIATE_BUFFER_TYPE(KP::ENGINE::UniformBufferObject);
INSTANTIATE_BUFFER_TYPE(KP::UTILS::lightData);

} // namespace ENGINE
} // namespace KP
