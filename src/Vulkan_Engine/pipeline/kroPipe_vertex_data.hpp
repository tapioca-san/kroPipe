#ifndef KROPIPE_VERTEX_DATA_HPP
#define KROPIPE_VERTEX_DATA_HPP

#include "../../kroPipe_depedence.hpp" // Necessário para glm, Vk structs

#include <array> // std::array
#include <glm/glm.hpp> // glm::vec3, glm::vec2

namespace KP {
namespace ENGINE {

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
} // namespace ENGINE
} // namespace KP

#endif//KROPIPE_VERTEX_DATA_HPP