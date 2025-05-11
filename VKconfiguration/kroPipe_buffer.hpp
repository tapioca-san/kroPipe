#ifndef BUFFER_H
#define BUFFER_H

#include "../VKkropipe/kroPipe_entity.hpp"
#include "../VKkropipe/kroPipe_struct.hpp"
#include "../VKkropipe/kroPipe_object.hpp"
#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_command.hpp"
#include "kroPipe_vertex.hpp"

namespace KP {
namespace BUFFER {

    inline void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) { //to send variables to the GPU memory. we use this with VKBUFFER to send our information.
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
        err = vkCreateBuffer(g_Device, &bufferInfo, nullptr, &buffer);
        check_vk_result(err, "failed to create buffer!");
    
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(g_Device, buffer, &memRequirements);
    
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = KP::VERTEX::OBJECT_vertex.findMemoryType(memRequirements.memoryTypeBits, properties);
    
        err = vkAllocateMemory(g_Device, &allocInfo, Allocator, &bufferMemory);
        check_vk_result(err, "failed to allocate buffer memory!");
    
        err = vkBindBufferMemory(g_Device, buffer, bufferMemory, 0);
        check_vk_result(err);
    }
    
    inline void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBuffer commandBuffer = KP::VERTEX::OBJECT_vertex.beginSingleTimeCommands();
    
        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    
        KP::VERTEX::OBJECT_vertex.endSingleTimeCommands(commandBuffer);
    }

struct UboStorage{

    public: ////////////////////////////////////////////////////////////////
        
        KP::STRUCT::uniformBuffers uniformBuffers;
        KP::STRUCT::UniformBufferObject UBO;
        int objectId;    

        size_t bufferSize = sizeof(KP::STRUCT::UniformBufferObject);
        
        /*
        */
        UboStorage() {
            this->objectId = createEntity(glm::vec3(0), 0.0f, false);
        }
            

        void create(){
            createUniformBuffers(uniformBuffers);
            KP::COMMANDBUFFER::OBJECT_command.createDescriptorPool(uniformBuffers.descriptorPool);
            createDescriptorSets(uniformBuffers);
        }
        
        void createDescriptorLayout(){
            createDescriptorSetLayout(uniformBuffers);
        }

        void update(){
            updateUniformBuffer(uniformBuffers,UBO,currentFrame);
        }
        
        void cleanUp(){
            cleanupBuffer(uniformBuffers);
        }

    private: ////////////////////////////////////////////////////////////////


        void createDescriptorSetLayout(KP::STRUCT::uniformBuffers &uniformBuffers) {
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

            err = vkCreateDescriptorSetLayout(g_Device, &layoutInfo, Allocator, &uniformBuffers.descriptorSetLayout);
            check_vk_result(err, "failed to create descriptor set layout!");

            setLayout.push_back(uniformBuffers.descriptorSetLayout);
        }

        void createUniformBuffers(KP::STRUCT::uniformBuffers &uniformBuffers) {
            VkDeviceSize bufferSize = sizeof(KP::STRUCT::UniformBufferObject);

            uniformBuffers.uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
            uniformBuffers.uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
            uniformBuffers.uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers.uniformBuffers[i], uniformBuffers.uniformBuffersMemory[i]);

                err = vkMapMemory(g_Device, uniformBuffers.uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffers.uniformBuffersMapped[i]);
                check_vk_result(err);
            }
        }

        void updateUniformBuffer(KP::STRUCT::uniformBuffers &uniformBuffers, KP::STRUCT::UniformBufferObject &ubo, const uint32_t &currentImage) {
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
            ubo.model = glm::mat4(1.0f);
            ubo.model = glm::translate(ubo.model, glm::vec3(allObjects[sortedID[objectId]]->data.Position.x, allObjects[sortedID[objectId]]->data.Position.y, allObjects[sortedID[objectId]]->data.Position.z));
            ubo.model = glm::rotate(ubo.model, glm::radians(allObjects[sortedID[objectId]]->data.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // rotation for X
            ubo.model = glm::rotate(ubo.model, glm::radians(allObjects[sortedID[objectId]]->data.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // rotation for Y
            ubo.model = glm::rotate(ubo.model, glm::radians(allObjects[sortedID[objectId]]->data.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // rotation for Z
            ubo.model = glm::scale(ubo.model, glm::vec3(allObjects[sortedID[objectId]]->data.Scale));
            
            
            if (flyMode) {
                ubo.view = cameraPlayer.GetViewMatrix();
            }
            else{
                ubo.view = glm::lookAt(glm::vec3(allObjects[sortedID[0]]->data.Position.x, allObjects[sortedID[0]]->data.Position.y, allObjects[sortedID[0]]->data.Position.z), glm::vec3(allObjects[sortedID[0]]->data.Position.x, allObjects[sortedID[0]]->data.Position.y, allObjects[sortedID[0]]->data.Position.z) + cameraPlayer.Front, cameraPlayer.Up);
            }
            ubo.proj = glm::perspective(glm::radians(cameraPlayer.Zoom), (float)swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 100.0f);
            ubo.proj[1][1] *= -1;
            
            memcpy(uniformBuffers.uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
        }

        

        void createDescriptorSets(KP::STRUCT::uniformBuffers &uniformBuffers) {

            std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, uniformBuffers.descriptorSetLayout);
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = uniformBuffers.descriptorPool;
            allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
            allocInfo.pSetLayouts = layouts.data();

            uniformBuffers. descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
            err = vkAllocateDescriptorSets(g_Device, &allocInfo, uniformBuffers.descriptorSets.data());
            check_vk_result(err, "failed to allocate descriptor sets!");

            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = uniformBuffers.uniformBuffers[i];
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(KP::STRUCT::UniformBufferObject);
                
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

                vkUpdateDescriptorSets(g_Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
            }
        }

        void cleanupBuffer(KP::STRUCT::uniformBuffers &uniformBuffers){
            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                vkDestroyBuffer(g_Device, uniformBuffers.uniformBuffers[i], Allocator);
                vkFreeMemory(g_Device, uniformBuffers.uniformBuffersMemory[i], Allocator);
            }

            vkDestroyDescriptorPool(g_Device, uniformBuffers.descriptorPool, Allocator);
            vkDestroyDescriptorSetLayout(g_Device, uniformBuffers.descriptorSetLayout, Allocator);
        }
};


} //BUFFER
} //KP
#endif//BUFFER_H