#include "kroPipe_queuFamilies.hpp"
#include "../debug/kroPipe_debug.hpp" // check_vk_result, fatalMessage, err
#include "../window/kroPipe_window.hpp" // OBJECT_window

namespace KP {
namespace ENGINE {

// Definição do método isComplete da struct QueueFamilyIndices
bool QueueFamilyIndices::isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
}

// Definição do objeto QueuFamilies (sem extern)
KP::ENGINE::QueuFamilies OBJECT_queuFamilies;

// Definição do método findQueuFamilies da classe QueuFamilies
QueueFamilyIndices QueuFamilies::findQueuFamilies(VkPhysicalDevice device){
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    if(queueFamilyCount == 0){
        throw std::runtime_error(KP::ENGINE::fatalMessage("failed to find a queue family!")); // Chamando fatalMessage do namespace
    }

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies){
        // Verificar suporte gráfico
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = i;
        }

        // Verificar suporte de apresentação para a surface
        VkBool32 presentSupport = false;
        // Usando OBJECT_window do namespace e check_vk_result/err do namespace
        KP::ENGINE::err = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, KP::ENGINE::OBJECT_window.VK_surface, &presentSupport);
        KP::ENGINE::check_vk_result(KP::ENGINE::err, "Failed to get physical device surface support"); // Adicionado mensagem de erro

        if (presentSupport) {
            indices.presentFamily = i;
        }

        // Se ambos os índices foram encontrados, podemos parar
        if (indices.isComplete())
            break;

        i++;
    }

    // Armazenar o índice gráfico encontrado no membro da classe
    // Certifique-se de que graphicsFamily tem um valor antes de acessar .value()
    if (indices.graphicsFamily.has_value()) {
        graphicsIndex = indices.graphicsFamily.value(); // Atualiza membro da classe
    } else {
         // Se não encontrou família gráfica, algo está errado, talvez lançar erro
         throw std::runtime_error(KP::ENGINE::fatalMessage("Failed to find a suitable graphics queue family!"));
    }


    return indices;
}

} // namespace ENGINE
} // namespace KP