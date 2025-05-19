#include "kroPipe_device.hpp"
#include "../queue/kroPipe_queuFamilies.hpp" // Incluído para OBJECT_queuFamilies
#include "../swapchain/kroPipe_swapchain.hpp" // Incluído para OBJECT_swapChain
#include "../debug/kroPipe_debug.hpp" // Incluído para fatalMessage, debug, VK_Allocator

// Não há definição para OBJECT_device aqui porque ele é inline no header

namespace KP {
namespace ENGINE {

// Definição dos métodos da classe Device
bool Device::checkDeviceExtensionSupport(VkPhysicalDevice &device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


bool Device::isDeviceSuitable(VkPhysicalDevice device) {
    // Usando objetos/funções do namespace KP::ENGINE
    KP::ENGINE::QueueFamilyIndices indices = KP::ENGINE::OBJECT_queuFamilies.findQueuFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    vkGetPhysicalDeviceProperties(device, &deviceProperties); // Atualiza membros da classe
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);   // Atualiza membros da classe

    vkGetPhysicalDeviceFeatures(device, &supportedFeatures); // Atualiza membros da classe
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        // Usando objeto do namespace KP::ENGINE
        KP::ENGINE::SwapChainSupportDetails swapChainSupport = KP::ENGINE::OBJECT_swapChain.querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    // Verificando features diretamente nos membros atualizados
    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void Device::pickPhysicalDevice(VkInstance& instance) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error(KP::ENGINE::fatalMessage("failed to find GPUs with Vulkan support!")); // Chamando fatalMessage do namespace
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) { // Chamando método da classe
            VK_PhysicalDevice = device; // Atualiza membro da classe
            break;
        }
    }

    if (VK_PhysicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error(KP::ENGINE::fatalMessage("failed to find a suitable GPU!")); // Chamando fatalMessage do namespace
    }
}

void Device::createLogicalDevice() {
    // Usando objeto/funções do namespace KP::ENGINE
    KP::ENGINE::QueueFamilyIndices indices = KP::ENGINE::OBJECT_queuFamilies.findQueuFamilies(VK_PhysicalDevice); // Usando membro da classe

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeaturesToEnable{}; // Features que queremos explicitamente habilitar
    deviceFeaturesToEnable.samplerAnisotropy = VK_TRUE; // Habilita anisotropia

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeaturesToEnable; // Habilitando features desejadas

    // Ativação de extensões (deviceExtensions é uma variável global/namespace)
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    // Ativação de validation layers (validationLayers é uma variável global/namespace)
    if (KP::ENGINE::debug) { // Usando variável debug do namespace
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    // Usando alocador global VK_Allocator do namespace
    if (vkCreateDevice(VK_PhysicalDevice, &createInfo, KP::ENGINE::VK_Allocator, &VK_Device) != VK_SUCCESS) { // Atualiza membro da classe
        throw std::runtime_error(KP::ENGINE::fatalMessage("failed to create logical device!")); // Chamando fatalMessage do namespace
    }

    // Obter as queues
    // vkGetDeviceQueue(VK_Device, indices.graphicsFamily.value(), 0, &graphicsQueue); // graphicsQueue não é membro de Device, mas sim de QueuFamilies
    // Obtenha graphicsQueue na classe QueuFamilies, se necessário.
    vkGetDeviceQueue(VK_Device, indices.presentFamily.value(), 0, &KP::ENGINE::OBJECT_queuFamilies.presentQueue); // Usando membro da classe QueuFamilies
}

} // namespace ENGINE
} // namespace KP