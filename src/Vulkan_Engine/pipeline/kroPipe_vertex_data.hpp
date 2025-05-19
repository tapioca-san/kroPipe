#ifndef KROPIPE_VERTEX_DATA_HPP
#define KROPIPE_VERTEX_DATA_HPP

#include "../../kroPipe_depedence.hpp" // Necessário para glm, Vk structs

#include <array> // std::array
#include <glm/glm.hpp> // glm::vec3, glm::vec2

namespace KP {
namespace ENGINE {

struct VertexVulkan {
    glm::vec3 Position;
    glm::vec3 color; // Seu código original tinha color antes de Normal
    glm::vec3 Normal; // Adicionado, estava no struct original mas não nos atributos
    glm::vec2 TexCoords;
    glm::vec3 Tangent; // Adicionado
    glm::vec3 Bitangent; // Adicionado

    // Garantindo que getAttributeDescriptions corresponde à ordem e tipos usados
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(VertexVulkan);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    // ATENÇÃO: Seus shaders esperam 3 atributos (location 0, 1, 2).
    // O código original definia Position, color, TexCoords.
    // Seus shaders precisam corresponder a isso.
    // Mantenha a ordem e a contagem que seus shaders esperam.
    // Assumindo que os shaders esperam Position (0), color (1), TexCoords (2).
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        // Atributo 0: Position
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VertexVulkan, Position);

        // Atributo 1: color
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VertexVulkan, color);

        // Atributo 2: TexCoords
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(VertexVulkan, TexCoords);

        // Se seus shaders esperam Normal, Tangent ou Bitangent, você precisará
        // aumentar o tamanho do array e adicionar entradas aqui, correspondendo
        // aos 'location' nos seus shaders.

        return attributeDescriptions;
    }

};

} // namespace ENGINE
} // namespace KP

#endif//KROPIPE_VERTEX_DATA_HPP