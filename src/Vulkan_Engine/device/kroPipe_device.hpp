#ifndef KROPIPE_DEVICE_HPP
#define KROPIPE_DEVICE_HPP

#include "../../kroPipe_depedence.hpp"
#include "../queue/kroPipe_queuFamilies.hpp"   // Necessário para QueueFamilyIndices
#include "../debug/kroPipe_debug.hpp"     // Necessário para debug/fatalMessage

// Constantes globais/namespace
const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};


namespace KP {
namespace ENGINE {

class Device{

public:
// Graphic cards informations
VkPhysicalDeviceProperties  deviceProperties = {}; // Inicializado
VkPhysicalDeviceFeatures    deviceFeatures = {};   // Inicializado
VkPhysicalDeviceFeatures    supportedFeatures = {};// Inicializado

VkPhysicalDevice            VK_PhysicalDevice = VK_NULL_HANDLE;
VkDevice                    VK_Device         = VK_NULL_HANDLE;


// Funções membro da classe Device
bool checkDeviceExtensionSupport(VkPhysicalDevice &device);
bool isDeviceSuitable(VkPhysicalDevice device);
void pickPhysicalDevice(VkInstance& instance);
void createLogicalDevice();

}; //CLASS DEVICE

// Objeto Device (definido inline no header - ok para ODR)
inline KP::ENGINE::Device OBJECT_device;

} // namespace ENGINE
} // namespace KP

#endif//KROPIPE_DEVICE_HPP