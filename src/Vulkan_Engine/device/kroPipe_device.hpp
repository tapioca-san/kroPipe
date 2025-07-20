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

extern std::vector<const char*> extensionAvailable;

class Device{

private:

VkPhysicalDeviceFeatures    supportedFeatures = {};
VkPhysicalDeviceProperties  deviceProperties = {}; 
VkPhysicalDeviceFeatures    deviceFeatures = {};   

VkPhysicalDevice            VK_PhysicalDevice;
VkDevice                    VK_Device;

public:

VkPhysicalDeviceProperties* getPointerDeviceProperties();
VkPhysicalDeviceFeatures* getPointerPhysicalDeviceFeatures();
VkPhysicalDeviceFeatures* getPointerSupportedFeatures();
VkPhysicalDevice* getPointerPhysicalDevice();
VkDevice* getPointerDevice();

bool checkDeviceExtensionSupport(VkPhysicalDevice &device);
bool isDeviceSuitable(VkPhysicalDevice device);
void pickPhysicalDevice(VkInstance& instance);
void createLogicalDevice();

}; 

extern KP::ENGINE::Device OBJECT_device;

} // namespace ENGINE
} // namespace KP

#endif//KROPIPE_DEVICE_HPP