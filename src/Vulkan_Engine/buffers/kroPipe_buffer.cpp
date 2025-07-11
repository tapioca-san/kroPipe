#include "../../Utils/object/kroPipe_object.hpp"
#include "../../Utils/camera/kroPipe_camera.hpp"
#include "../swapchain/kroPipe_swapchain.hpp"
#include "../texture/kroPipe_texture.hpp"
#include "../command/kroPipe_command.hpp"
#include "../device/kroPipe_device.hpp"
#include "../vertex/kroPipe_vertex.hpp"
#include "../render/kroPipe_render.hpp"
#include "../debug/kroPipe_debug.hpp"
#include "kroPipe_buffer.hpp"
namespace KP {
namespace ENGINE {
    
std::vector<VkDescriptorSetLayout> setLayout;

uint32_t currentFrame = 0;

KP::ENGINE::UboStorage::UboStorage(uint32_t &objectID){
    this->objectId = objectID;    
}

void createDescriptorSetLayout(KP::ENGINE::UniformBuffers &uniformBuffers) {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();
    err = vkCreateDescriptorSetLayout(*KP::ENGINE::OBJECT_device.getPointerDevice(), &layoutInfo, KP::ENGINE::VK_Allocator, &uniformBuffers.descriptorSetLayout);
    check_vk_result(err, "failed to create descriptor set layout!");
    setLayout.push_back(uniformBuffers.descriptorSetLayout);
}

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) { //to send variables to the GPU memory. we use this with VKBUFFER to send our information.
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    err = vkCreateBuffer(*KP::ENGINE::OBJECT_device.getPointerDevice(), &bufferInfo, nullptr, &buffer);
    check_vk_result(err, "failed to create buffer!");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(*KP::ENGINE::OBJECT_device.getPointerDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = KP::ENGINE::OBJECT_vertex.findMemoryType(memRequirements.memoryTypeBits, properties);

    err = vkAllocateMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(), &allocInfo, KP::ENGINE::VK_Allocator, &bufferMemory);
    check_vk_result(err, "failed to allocate buffer memory!");

    err = vkBindBufferMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(), buffer, bufferMemory, 0);
    check_vk_result(err);
}
    
void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = KP::ENGINE::OBJECT_vertex.beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    KP::ENGINE::OBJECT_vertex.endSingleTimeCommands(commandBuffer);
}

void KP::ENGINE::UboStorage::create(){
    createUniformBuffers(uniformBuffers);
    KP::ENGINE::OBJECT_command.createDescriptorPool(uniformBuffers.descriptorPool);
    createDescriptorSets(uniformBuffers);
}

void KP::ENGINE::UboStorage::createDescriptorLayout(){
    createDescriptorSetLayout(uniformBuffers);
}

void KP::ENGINE::UboStorage::update(){
    updateUniformBuffer(uniformBuffers,UBO,currentFrame);
}

void KP::ENGINE::UboStorage::cleanUp(){
    cleanupBuffer(uniformBuffers);
}

void KP::ENGINE::UboStorage::createUniformBuffers(KP::ENGINE::UniformBuffers &uniformBuffers) {
    VkDeviceSize bufferSize = sizeof(KP::ENGINE::UniformBufferObject);
    uniformBuffers.uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffers.uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffers.uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers.uniformBuffers[i], uniformBuffers.uniformBuffersMemory[i]);
        err = vkMapMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(), uniformBuffers.uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffers.uniformBuffersMapped[i]);
        check_vk_result(err);
    }
}

void KP::ENGINE::UboStorage::updateUniformBuffer(KP::ENGINE::UniformBuffers &uniformBuffers, KP::ENGINE::UniformBufferObject &ubo, const uint32_t &currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(ubo.model, glm::vec3(11, KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.y, KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.z));
    ubo.model = glm::rotate(ubo.model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(-0.5f, 1.0f, 0.0f));
    //ubo.view = glm::lookAt(glm::vec3(KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.x, KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.y, KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.z), glm::vec3(KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.x, KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.y, KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.z) + KP::UTILS::cameraPlayer.Front, KP::UTILS::cameraPlayer.Up);
    //ubo.proj = glm::perspective(glm::radians(KP::UTILS::cameraPlayer.Zoom), (float)KP::ENGINE::swapChainExtent.width / (float)KP::ENGINE::swapChainExtent.height, 0.1f, 100.0f);
    ubo.proj[1][1] *= -1;
        

    
    memcpy(uniformBuffers.uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void KP::ENGINE::UboStorage::createDescriptorSets(KP::ENGINE::UniformBuffers &uniformBuffers) {
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, uniformBuffers.descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = uniformBuffers.descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();
    uniformBuffers. descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    err = vkAllocateDescriptorSets(*KP::ENGINE::OBJECT_device.getPointerDevice(), &allocInfo, uniformBuffers.descriptorSets.data());
    check_vk_result(err, "failed to allocate descriptor sets!");
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers.uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(KP::ENGINE::UniformBufferObject);
        
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = textureImageView;
        imageInfo.sampler = textureSampler;
        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = uniformBuffers.descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;
        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = uniformBuffers.descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;
        vkUpdateDescriptorSets(*KP::ENGINE::OBJECT_device.getPointerDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void KP::ENGINE::UboStorage::cleanupBuffer(KP::ENGINE::UniformBuffers &uniformBuffers){
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(*KP::ENGINE::OBJECT_device.getPointerDevice(), uniformBuffers.uniformBuffers[i], KP::ENGINE::VK_Allocator);
        vkFreeMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(), uniformBuffers.uniformBuffersMemory[i], KP::ENGINE::VK_Allocator);
    }
    vkDestroyDescriptorPool(*KP::ENGINE::OBJECT_device.getPointerDevice(), uniformBuffers.descriptorPool, KP::ENGINE::VK_Allocator);
    vkDestroyDescriptorSetLayout(*KP::ENGINE::OBJECT_device.getPointerDevice(), uniformBuffers.descriptorSetLayout, KP::ENGINE::VK_Allocator);
}

uint32_t aa = 0;
KP::ENGINE::UboStorage OBJECT_sceneUBO(aa);

}//ENGINE
}//KP