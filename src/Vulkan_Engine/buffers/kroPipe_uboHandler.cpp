#include "../../Utils/object/kroPipe_object.hpp"
#include "../../Utils/object/kroPipe_light.hpp"
#include "../swapchain/kroPipe_swapchain.hpp"
#include "../texture/kroPipe_texture.hpp"
#include "../device/kroPipe_device.hpp"
#include "../render/kroPipe_render.hpp"
#include "kroPipe_bufferManager.hpp"
#include "kroPipe_descriptorSet.hpp"
#include "kroPipe_uboHandler.hpp"


namespace KP {
namespace ENGINE {


std::vector<VkDescriptorSetLayout> setLayout;

uint32_t currentFrame = 0;
    
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
    VkDescriptorSetLayoutBinding lightLayoutBinding{};
    lightLayoutBinding.binding = 2;
    lightLayoutBinding.descriptorCount = 1;
    lightLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    lightLayoutBinding.pImmutableSamplers = nullptr;
    lightLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    
    std::array<VkDescriptorSetLayoutBinding, 3> bindings = {uboLayoutBinding, samplerLayoutBinding, lightLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();
    err = vkCreateDescriptorSetLayout(*KP::ENGINE::OBJECT_device.getPointerDevice(), &layoutInfo, KP::ENGINE::VK_Allocator, &uniformBuffers.descriptorSetLayout);
    check_vk_result(err, "failed to create descriptor set layout!");
    setLayout.push_back(uniformBuffers.descriptorSetLayout);
}

KP::ENGINE::UboStorage::UboStorage(uint32_t &objectID){
    this->objectId = objectID;    
}

void KP::ENGINE::UboStorage::create(){
    createUniformBuffers(uniformBuffers);
    KP::ENGINE::OBJECT_command.createDescriptorPool(uniformBuffers.descriptorPool);
    OBJECT_DescriptorSet.createDescriptorSets(uniformBuffers);
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
    ubo.model = glm::translate(ubo.model, glm::vec3(KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.x, KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.y, KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Position.z));
    ubo.model = glm::rotate(ubo.model, glm::radians(KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // rotation for X
    ubo.model = glm::rotate(ubo.model, glm::radians(KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // rotation for Y
    ubo.model = glm::rotate(ubo.model, glm::radians(KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // rotation for Z
    ubo.model = glm::scale(ubo.model, glm::vec3(KP::UTILS::OBJECT_objectsManager.getObjectByID(objectId)->getData().Scale));
    ubo.view = glm::lookAt(glm::vec3(KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0))->getData().Position.x, KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0))->getData().Position.y, KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0))->getData().Position.z), glm::vec3(KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0))->getData().Position.x, KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0))->getData().Position.y, KP::UTILS::OBJECT_objectsManager.getObjectByID(*KP::UTILS::OBJECT_objectsManager.getCamerasID(0))->getData().Position.z) + KP::UTILS::cameraPlayer.Front, KP::UTILS::cameraPlayer.Up);
    ubo.proj = glm::perspective(glm::radians(KP::UTILS::cameraPlayer.Zoom), (float)KP::ENGINE::swapChainExtent.width / (float)KP::ENGINE::swapChainExtent.height, 0.1f, 100.0f);
    ubo.proj[1][1] *= -1;
        
    memcpy(uniformBuffers.uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
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

} //ENGINE
} //KP