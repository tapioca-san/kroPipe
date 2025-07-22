#ifndef KROPIPE_VERTEX_DATA_HPP
#define KROPIPE_VERTEX_DATA_HPP

#include "../../kroPipe_depedence.hpp" 

#define MAX_BONE_INFLUENCE 4

namespace KP {
namespace ENGINE {

struct VertexVulkan {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 color;
    glm::vec4 vertexColors;

    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
    
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

    static std::array<VkVertexInputAttributeDescription, 9> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 9> attributeDescriptions{};

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
        
        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;;
        attributeDescriptions[3].offset = offsetof(VertexVulkan, vertexColors);
        
        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(VertexVulkan, Normal);
        
        attributeDescriptions[5].binding = 0;
        attributeDescriptions[5].location = 5;
        attributeDescriptions[5].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[5].offset = offsetof(VertexVulkan, Tangent);

        attributeDescriptions[6].binding = 0;
        attributeDescriptions[6].location = 6;
        attributeDescriptions[6].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[6].offset = offsetof(VertexVulkan, Bitangent);

        attributeDescriptions[7].binding = 0;
        attributeDescriptions[7].location = 7;
        attributeDescriptions[7].format = VK_FORMAT_R32G32B32A32_SFLOAT;;
        attributeDescriptions[7].offset = offsetof(VertexVulkan, boneIDs);

        attributeDescriptions[8].binding = 0;
        attributeDescriptions[8].location = 8;
        attributeDescriptions[8].format = VK_FORMAT_R32G32B32A32_SFLOAT;;
        attributeDescriptions[8].offset = offsetof(VertexVulkan, weights);

        return attributeDescriptions;
    }
    
};
} // namespace ENGINE
} // namespace KP

#endif//KROPIPE_VERTEX_DATA_HPP