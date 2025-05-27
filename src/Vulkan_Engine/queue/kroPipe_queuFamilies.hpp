#ifndef KROPIPE_QUEUFAMILIES_HPP
#define KROPIPE_QUEUFAMILIES_HPP

#include "../../kroPipe_depedence.hpp"
//#include "../debug/kroPipe_debug.hpp" // check_vk_result, fatalMessage, err
//#include "../window/kroPipe_window.hpp" // OBJECT_window (para vkGetPhysicalDeviceSurfaceSupportKHR)


namespace KP {
namespace ENGINE {

// Struct para armazenar índices das famílias de queues
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete(); // Método para verificar se ambos os índices foram encontrados
};

// Classe QueuFamilies
class QueuFamilies{

public:
    // Handles para as queues obtidas
    VkQueue presentQueue = VK_NULL_HANDLE; // Inicializado
    VkQueue graphicsQueue = VK_NULL_HANDLE; // Inicializado

    // Índice da família de queue gráfica
    uint32_t graphicsIndex = -1; // Inicializado com valor inválido

    // Método para encontrar os índices das famílias de queues
    QueueFamilyIndices findQueuFamilies(VkPhysicalDevice device);
};

// Objeto QueuFamilies (declarado extern)
extern KP::ENGINE::QueuFamilies OBJECT_queuFamilies;

} // namespace QUEUFAMILIES
} // namespace KP

#endif //KROPIPE_QUEUFAMILIES_HPP