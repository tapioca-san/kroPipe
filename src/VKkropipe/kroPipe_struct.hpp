#ifndef STRUCT_H
#define STRUCT_H

#include "../kroPipe_include.hpp"



namespace KP {

namespace STRUCT {

struct VertexVulkan {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 color;
    
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(VertexVulkan);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VertexVulkan, Position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VertexVulkan, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(VertexVulkan, TexCoords);

        return attributeDescriptions;
    }
    
};

struct Mesh {
    std::vector<VertexVulkan> vertices;
    std::vector<uint16_t> indices;
    
    Mesh(std::vector<VertexVulkan> v, std::vector<uint16_t> i) 
    : vertices(std::move(v)), indices(std::move(i)) {}
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};



struct shaderModule {
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    
    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};



struct uniformBuffers{
    std::vector<VkBuffer>               uniformBuffers;
    std::vector<VkDeviceMemory>         uniformBuffersMemory;
    std::vector<void*>                  uniformBuffersMapped;  
    
    std::vector<VkDescriptorSet>        descriptorSets;
    VkDescriptorPool                    descriptorPool;
    VkDescriptorSetLayout               descriptorSetLayout;

};

struct VAO {

    std::vector<VkBuffer> vertexBuffers;
    std::vector<VkDeviceMemory> vertexBufferMemorys;
    
    std::vector<VkBuffer> indexBuffers;
    std::vector<VkDeviceMemory> indexBufferMemorys;
    
    std::vector<KP::STRUCT::Mesh> meshes;
    
    KP::STRUCT::UniformBufferObject UBO;
    
};

} // STRUCT
} // KP

KP::STRUCT::uniformBuffers uniformBuffers2;
KP::STRUCT::QueueFamilyIndices indices;


#endif//STRUCT_H

