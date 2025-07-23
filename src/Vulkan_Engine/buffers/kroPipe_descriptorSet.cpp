#include "../../Utils/object/kroPipe_light.hpp"
#include "../texture/kroPipe_texture.hpp"
#include "../render/kroPipe_render.hpp"
#include "kroPipe_descriptorSet.hpp"
#include "kroPipe_bufferManager.hpp"


namespace KP {
namespace ENGINE {

void DescriptorSet::createDescriptorSets(KP::ENGINE::UniformBuffers &uniformBuffers) {
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

        VkDescriptorBufferInfo lightBufferInfo{};
        lightBufferInfo.buffer = KP::ENGINE::OBJECT_bufferManager.getUniformBufferByID(KP::UTILS::lightTest.getBufferID()); 
        lightBufferInfo.offset = 0;
        lightBufferInfo.range = sizeof(KP::UTILS::lightData);

        std::array<VkWriteDescriptorSet, 3> descriptorWrites{};
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

        descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[2].dstSet = uniformBuffers.descriptorSets[i];
        descriptorWrites[2].dstBinding = 2;
        descriptorWrites[2].dstArrayElement = 0;
        descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[2].descriptorCount = 1;
        descriptorWrites[2].pBufferInfo = &lightBufferInfo;
        
        vkUpdateDescriptorSets(*KP::ENGINE::OBJECT_device.getPointerDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

KP::ENGINE::DescriptorSet OBJECT_DescriptorSet;

} //BUFFER
} //KP
